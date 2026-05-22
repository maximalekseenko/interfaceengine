// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_OBJECT_H_
#define SRC_INTERFACE_LUMEN_OBJECT_OBJECT_H_

#include <SDL3/SDL_render.h>

#include <functional>
#include <istream>
#include <memory>
#include <string>

#include "interface/dll.h"

namespace interfaceengine::lumen::object {

class INTERFACE_INTERNAL
    Object {  // TODO(necromax): make more const functions among modules
 public:
  using Ptr = std::unique_ptr<Object>;
  using getFileFn = std::function<std::unique_ptr<std::istream>(
      const std::string& file_name)>;
  using getObjectFn
      = std::function<const Object*(const std::string& object_name)>;

 public:
  static Ptr CreateOfType(const std::string& type, const std::string& name);

 public:
  virtual ~Object() = default;
  virtual void Load(const std::string& meta, const getFileFn& get_file_fn,
                    const getObjectFn& get_object_fn, SDL_Renderer* renderer) {}

  std::string type() { return type_; }
  std::string name() { return name_; }

 private:
  std::string type_;
  std::string name_;
};

}  // namespace interfaceengine::lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_OBJECT_H_
