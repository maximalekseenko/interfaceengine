// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_LUMEN_IMAGE_H_
#define SRC_INTERFACE_LUMEN_OBJECT_LUMEN_IMAGE_H_

#include <SDL3/SDL_render.h>

#include <string>

#include "interface/dll.h"
#include "interface/lumen/object/lumen.h"

namespace interfaceengine::lumen::object {

class INTERFACE_INTERNAL LumenImage : public Lumen {
 public:
  ~LumenImage();

 public:
  void Load(const std::string& meta, const getFileFn& get_file_fn,
            const getObjectFn& get_object_fn, SDL_Renderer* renderer) override;

  SDL_Texture* GetTexture(std::string data) const override { return texture_; }

 private:
  SDL_Texture* texture_;
};

}  // namespace interfaceengine::lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_LUMEN_IMAGE_H_
