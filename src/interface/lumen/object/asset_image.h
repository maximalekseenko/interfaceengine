// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_ASSET_IMAGE_H_
#define SRC_INTERFACE_LUMEN_OBJECT_ASSET_IMAGE_H_

#include <SDL3/SDL_surface.h>

#include <istream>
#include <string>

#include "interface/dll.h"
#include "interface/lumen/object/asset.h"

namespace interfaceengine::lumen::object {

class INTERFACE_INTERNAL AssetImage : public Asset {
 public:
  struct SurfaceData {
    bool is_pixel{false};
    bool has_color_key{false};
    uint16_t color_key{0};
  };

 public:
  AssetImage() {}
  virtual ~AssetImage();

 public:
  void Load(const std::string& meta, const getFileFn& get_file_fn,
            const getObjectFn& get_object_fn, SDL_Renderer* renderer) override;

  SDL_Surface* surface() const { return surface_; }

  const SurfaceData& surface_data() const { return surface_data_; }

 private:
  SurfaceData surface_data_;
  SDL_Surface* surface_{nullptr};
};

}  // namespace interfaceengine::lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_ASSET_IMAGE_H_
