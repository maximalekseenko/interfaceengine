// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/package/package.h"

#include <necroutils/colexc.h>

#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "interface/lumen/object/lumen.h"
#include "interface/lumen/package/exceptions.h"
#include "interface/lumen/package/package_header.h"

namespace interfaceengine::lumen::package {

struct FileLocation {
  std::streampos pos;
  std::size_t size;
};

template <typename T>
void ReadCasted(std::ifstream& in_file, T& content) {
  in_file.read(reinterpret_cast<char*>(&content), sizeof(content));
}

void LumPackage::LoadPackage(const std::string& package_path,
                             SDL_Renderer* renderer) {
  size_t last_slash = package_path.find_last_of("/\\");

  if (last_slash == std::string::npos) {
    name_ = package_path;
  } else {
    name_ = package_path.substr(last_slash + 1);
  }
  size_t last_dot = name_.find_last_of('.');
  if (last_dot != std::string::npos && last_dot != 0)
    name_ = name_.substr(0, last_dot);

  std::ifstream lum_file(package_path);
  std::unordered_map<std::string, FileLocation> file_index;
  std::vector<std::string> meta_lines;

  if (!lum_file.is_open())
    throw LumPackageException("LumPackage lum not found at: \"" + package_path
                              + "\".");

  lumen::package::PackageHeader header;

  lum_file.read(header.magic.data(), header.magic.size());
  ReadCasted(lum_file, header.version);
  ReadCasted(lum_file, header.files_count);
  ReadCasted(lum_file, header.meta_lines_count);

  // Gather file locations.
  for (PackageHeader::ElemCount i = 0; i < header.files_count; ++i) {
    lumen::package::PackageHeader::FileNameSize file_name_size;
    std::string file_name;
    lumen::package::PackageHeader::FileSize file_size;

    ReadCasted(lum_file, file_name_size);

    file_name.resize(file_name_size);
    lum_file.read(file_name.data(), file_name_size);

    ReadCasted(lum_file, file_size);

    std::streampos file_data_pos = lum_file.tellg();

    file_index[file_name] = {.pos = file_data_pos, .size = file_size};

    lum_file.seekg(file_size, std::ios::cur);
  }

  // Gather meta lines.
  for (PackageHeader::ElemCount i = 0; i < header.meta_lines_count; ++i) {
    lumen::package::PackageHeader::FileNameSize meta_line_size;
    std::string meta_line;

    ReadCasted(lum_file, meta_line_size);

    meta_line = std::string(meta_line_size, '\0');
    lum_file.read(meta_line.data(), meta_line_size);

    meta_lines.push_back(std::move(meta_line));
  }

  // Register objects.
  auto file_provider
      = [&, file_index = std::move(file_index)](
            const std::string& file_name) -> std::unique_ptr<std::istream> {
    auto it = file_index.find(file_name);

    if (it == file_index.end()) return nullptr;

    auto [offset, size] = it->second;

    lum_file.clear();
    lum_file.seekg(offset);

    std::string buffer(size, '\0');
    lum_file.read(buffer.data(), size);

    auto stream = std::make_unique<std::istringstream>(std::move(buffer));
    return stream;
  };

  for (const auto& meta_line : meta_lines)
    AddMeta(meta_line, file_provider, renderer);
}

void LumPackage::AddMeta(const std::string& meta_line,
                         const object::Object::getFileFn& get_file_fn,
                         SDL_Renderer* renderer) {
  std::stringstream meta_stream(meta_line);

  std::string object_type, object_name;

  if (!(meta_stream >> object_type))
    throw MetaEmtpyLine(
        "LumPackage had failed to process meta due to:\n"
        "It is empty.");

  else if (object_type.starts_with('-'))
    throw MetaCommentedLine(
        "LumPackage had failed to process meta due to:\n"
        "It is commented.");

  if (!(meta_stream >> object_name))
    throw InvalidMetaStructure(
        "LumPackage had failed to process meta due to:\n"
        "No object name specified.");

  auto new_asset = object::Object::CreateOfType(object_type, object_name);

  new_asset->Load(
      meta_line, get_file_fn,
      [this](const std::string& name) { return GetObject(name); }, renderer);

  AddObject(std::move(new_asset));
}

SDL_Texture* LumPackage::GetLumenTexture(std::string lumen_name) {
  auto it = loaded_objects_.find(lumen_name);
  if (it == loaded_objects_.end())
    throw ObjectNotFound("LumPackage failed to find lumen \"" + lumen_name
                         + "\".");  // TODO(necromax): better exception texts.
                                    // (at least tell what package, duh)

  auto lumen = dynamic_cast<const object::Lumen*>(it->second.get());
  if (lumen == nullptr)
    throw ObjectNotFound("LumPackage found object \"" + lumen_name
                         + "\", but it wasn't a lumen.");
  return lumen->GetTexture("");
}

const object::Object* LumPackage::GetObject(const std::string& object_name) {
  auto it = loaded_objects_.find(object_name);
  if (it != loaded_objects_.end()) return it->second.get();
  return nullptr;
}

void LumPackage::AddObject(object::Object::Ptr object) {
  if (object == nullptr)
    throw ObjectNotFound("LumPackage tried to add null object.");

  std::string object_type = object->type();
  std::string object_name = object->name();
  auto emplace_result
      = loaded_objects_.try_emplace(object_name, std::move(object));

  if (!emplace_result.second) {
    throw DuplicateObject("LumPackage got a duplicate object with name \""
                          + object_name + "\".");
  }
}

}  // namespace interfaceengine::lumen::package
