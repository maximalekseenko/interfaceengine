// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_H_
#define SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "interface/dll.h"
#include "interface/lumen/object/object.h"

namespace interfaceengine::lumen::package {

class INTERFACE_INTERNAL LumPackage {
 public:
  friend class LumPackageLoader;
  using Ptr = std::unique_ptr<LumPackage>;

 public:
  LumPackage() = default;

 public:
  SDL_Texture* GetLumenTexture(const std::string& lumen_name,
                               const std::string& data);

  std::string name() { return name_; }

  void AddMeta(const std::string& meta_line,
               const object::Object::getFileFn& get_file_fn,
               struct SDL_Renderer* renderer);

 private:
  const object::Object* GetObject(const std::string& object_name);
  void AddObject(object::Object::Ptr object);

 private:
  std::string name_;
  std::unordered_map<std::string, object::Object::Ptr> loaded_objects_;
};

}  // namespace interfaceengine::lumen::package

#endif  // SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_H_
