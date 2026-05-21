// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/object/object.h"

#include <memory>
#include <string>

#include "interface/lumen/object/asset_image.h"
#include "interface/lumen/object/exceptions.h"
#include "interface/lumen/object/lumen_image.h"

namespace lumen::object {

Object::Ptr Object::CreateOfType(const std::string& type,
                                 const std::string& name) {
  Object::Ptr new_object;

  // Create by type
  if (type == "image")
    new_object = std::make_unique<AssetImage>();
  else if (type == "lumimage")
    new_object = std::make_unique<LumenImage>();
  else
    throw ObjectException("Cannot create object of unknown type \"" + type
                          + "\".");

  // Fill data.
  new_object->name_ = name;
  new_object->type_ = type;

  // Return.
  return new_object;
}

}  // namespace lumen::object
