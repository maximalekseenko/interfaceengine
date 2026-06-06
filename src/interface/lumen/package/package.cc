// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/package/package.h"

#include <sstream>
#include <string>
#include <utility>

#include "interface/lumen/object/lumen.h"
#include "interface/lumen/package/exceptions.h"

namespace interfaceengine::lumen::package {

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

SDL_Texture* LumPackage::GetLumenTexture(const std::string& lumen_name,
                                         const std::string& data) {
  auto it = loaded_objects_.find(lumen_name);
  if (it == loaded_objects_.end())
    throw ObjectNotFound("LumPackage failed to find lumen \"" + lumen_name
                         + "\".");  // TODO(necromax): better exception texts.
                                    // (at least tell what package, duh)

  auto lumen = dynamic_cast<const object::Lumen*>(it->second.get());
  if (lumen == nullptr)
    throw ObjectNotFound("LumPackage found object \"" + lumen_name
                         + "\", but it wasn't a lumen.");
  return lumen->GetTexture(data);
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
