// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/package/loader.h"

#include <necroutils/colexc.h>

#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "interface/lumen/object/lumen.h"
#include "interface/lumen/package/exceptions.h"
#include "interface/lumen/package/package_header.h"

namespace interfaceengine::lumen::package {

namespace {

struct FileLocation {
  std::streampos pos;
  std::size_t size;
};

template <typename T>
void WriteContainer(std::ofstream& out_file, const T& container) {
  out_file.write(container.data(), container.size());
}

template <typename T>
void WriteCasted(std::ofstream& out_file, const T& content) {
  out_file.write(reinterpret_cast<const char*>(&content), sizeof(content));
}

template <typename T>
void ReadCasted(std::ifstream& in_file, T& content) {
  in_file.read(reinterpret_cast<char*>(&content), sizeof(content));
}

void ParsePath(std::string path, std::string* out_parent_path,
               std::string* out_name, std::string* out_extension) {
  if (path.empty()) {
    if (out_parent_path) *out_parent_path = ".";
    if (out_name) *out_name = "";
    if (out_extension) *out_extension = "";
    return;
  }

  size_t last_slash = path.find_last_of("/\\");
  std::string name_with_ext;

  if (last_slash == std::string::npos) {
    if (out_parent_path) *out_parent_path = ".";
    name_with_ext = path;
  } else {
    if (out_parent_path)
      *out_parent_path
          = (last_slash == 0) ? path.substr(0, 1) : path.substr(0, last_slash);
    name_with_ext = path.substr(last_slash + 1);
  }

  size_t last_dot = name_with_ext.find_last_of('.');
  if (last_dot == std::string::npos) {
    if (out_name) *out_name = name_with_ext;
    if (out_extension) *out_extension = "";
  } else {
    if (out_extension) *out_extension = name_with_ext.substr(last_dot + 1);
    if (out_name) *out_name = name_with_ext.substr(0, last_dot);
  }
}

LumPackage::Ptr CreatePackageFromLumdata(
    const std::string& lumdata_path, object::Object::getFileFn file_provider,
    std::vector<std::string>* valid_meta_lines, SDL_Renderer* renderer) {
  LumPackage::Ptr new_package = std::make_unique<LumPackage>();

  std::string package_folder;
  ParsePath(lumdata_path, &package_folder, nullptr, nullptr);

  std::ifstream lumdata_file;
  std::string assets_folder, package_name;

  // Open input (lumdata) file.
  lumdata_file.open(lumdata_path, std::ios::binary);
  if (!lumdata_file.is_open())
    throw "Cannot compile lumdata file \"" + lumdata_path
        + "\": File does not exist.";
  // TODO(necromax): exceptions.

  // Load objects from lumdata file.
  CollectiveException col_exc("LumPackage had finished loading lumdata file \""
                              + lumdata_path + "\" with following exceptions:");

  int line_index = 0;
  for (std::string lumdata_line; std::getline(lumdata_file, lumdata_line);
       ++line_index) {
    try {
      new_package->AddMeta(lumdata_line, file_provider, renderer);
      if (valid_meta_lines) valid_meta_lines->push_back(lumdata_line);
    } catch (const interfaceengine::lumen::package::MetaEmtpyLine& e) {
    } catch (const interfaceengine::lumen::package::MetaCommentedLine& e) {
    } catch (const std::exception& e) {
      col_exc.Add(std::make_exception_ptr(InvalidLumdataError(
          "Failed to process line " + std::to_string(line_index) + " due to:\n"
          + e.what())));
    } catch (...) {
      col_exc.Add(std::make_exception_ptr(InvalidLumdataError(
          "Failed to process line " + std::to_string(line_index) + " due to:\n"
          + "Unknown error.")));
    }
  }

  // Check for errors.
  col_exc.ThrowIfCollected();

  // Return.
  return new_package;
}

}  // namespace

LumPackage::Ptr LumPackageLoader::Load(const std::string& package_path,
                                       SDL_Renderer* renderer) {
  std::string extention;
  ParsePath(package_path, nullptr, nullptr, &extention);
  if (extention == "") return LoadFromFolder(package_path, renderer);
  if (extention == "lum") return LoadFromLumFile(package_path, renderer);
  if (extention == "lumdata")
    return LoadFromLumdataFile(package_path, renderer);

  throw;  // TODO(necromax): exception.
}

LumPackage::Ptr LumPackageLoader::LoadFromLumFile(
    const std::string& package_path, SDL_Renderer* renderer) {
  LumPackage::Ptr new_package = std::make_unique<LumPackage>();

  ParsePath(package_path, nullptr, &new_package->name_, nullptr);

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
    new_package->AddMeta(meta_line, file_provider, renderer);

  // Return result.
  return new_package;
}

LumPackage::Ptr LumPackageLoader::LoadFromLumdataFile(
    const std::string& package_path, SDL_Renderer* renderer) {
  std::string package_folder;
  ParsePath(package_path, &package_folder, nullptr, nullptr);

  auto file_provider = [&package_folder](const std::string& file_name) {
    return std::make_unique<std::ifstream>(package_folder + '/' + file_name);
  };
  return CreatePackageFromLumdata(package_path, file_provider, nullptr,
                                  renderer);
}

LumPackage::Ptr LumPackageLoader::LoadFromFolder(
    const std::string& package_path, SDL_Renderer* renderer) {
  throw;
}

void LumPackageLoader::CompileFromLumdataFile(const std::string& package_path,
                                              std::string out_path,
                                              SDL_Renderer* renderer) {
  std::unordered_set<std::string> required_file_names;
  std::vector<std::string> valid_meta_lines;

  std::ofstream lum_file;

  std::string package_folder, package_name;
  ParsePath(package_path, &package_folder, &package_name, nullptr);

  // Load package and gather files and lines.
  auto file_provider = [&required_file_names,
                        &package_folder](const std::string& file_name) {
    required_file_names.emplace(file_name);
    return std::make_unique<std::ifstream>(package_folder + '/' + file_name);
  };
  CreatePackageFromLumdata(package_path, file_provider, &valid_meta_lines,
                           renderer);

  // Open output (lum) file.
  if (!out_path.ends_with(".lum")) out_path += '/' + package_name + ".lum";

  lum_file.open(out_path, std::ios::binary);
  if (!lum_file.is_open())
    throw "Cannot compile lumdata file \"" + package_path
        + "\": Cannot create output file \"" + out_path + "\".";

  // Write header.
  interfaceengine::lumen::package::PackageHeader header{
      .magic{'L', 'U', 'M', '0'},
      .version = 0,
      .files_count = required_file_names.size(),
      .meta_lines_count = valid_meta_lines.size()};

  WriteContainer(lum_file, header.magic);
  WriteCasted(lum_file, header.version);
  WriteCasted(lum_file, header.files_count);
  WriteCasted(lum_file, header.meta_lines_count);

  // Write files.
  for (const auto& file_name : required_file_names) {
    std::ifstream file_stream(package_folder + '/' + file_name);
    interfaceengine::lumen::package::PackageHeader::FileNameSize file_name_size;
    interfaceengine::lumen::package::PackageHeader::FileSize file_size;

    file_name_size = file_name.size();

    file_stream.seekg(0, std::ios::end);
    file_size = static_cast<uint32_t>(file_stream.tellg());
    file_stream.seekg(0, std::ios::beg);

    WriteCasted(lum_file, file_name_size);
    WriteContainer(lum_file, file_name);
    WriteCasted(lum_file, file_size);

    std::vector<char> buffer(64 * 1024);
    while (file_stream) {
      file_stream.read(buffer.data(), buffer.size());
      std::streamsize n = file_stream.gcount();
      lum_file.write(buffer.data(), n);
    }
  }

  // Write meta.
  for (const auto& meta_line : valid_meta_lines) {
    interfaceengine::lumen::package::PackageHeader::MetaSize meta_line_size;

    meta_line_size = meta_line.size();

    WriteCasted(lum_file, meta_line_size);
    WriteContainer(lum_file, meta_line);
  }
}

}  // namespace interfaceengine::lumen::package
