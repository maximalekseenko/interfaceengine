// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_LUMEN_H_
#define SRC_INTERFACE_LUMEN_OBJECT_LUMEN_H_

#include <SDL3/SDL_render.h>

#include <string>

#include "interface/dll.h"
#include "interface/lumen/object/object.h"

namespace lumen::object {

class DLL_LOCAL Lumen : public Object {
 public:
  virtual ~Lumen() = default;

 public:
  virtual SDL_Texture* GetTexture(std::string data) const = 0;
};

}  // namespace lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_LUMEN_H_
