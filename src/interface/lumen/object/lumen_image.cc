// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/object/lumen_image.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <typeinfo>

#include "interface/lumen/object/asset_image.h"
#include "interface/lumen/object/exceptions.h"
#include "interface/lumen/object/sdl_extract.h"

namespace lumen::object {

LumenImage::~LumenImage() {
  if (texture_ != nullptr) SDL_DestroyTexture(texture_);
}

void LumenImage::Load(const std::string& meta, const getFileFn& get_file_fn,
                      const getObjectFn& get_object_fn,
                      SDL_Renderer* renderer) {
  std::stringstream meta_stream(meta);
  std::string meta_word;
  std::string image_name;
  uint32_t extract_x, extract_y, extract_w, extract_h;

  // Skip type and name.
  meta_stream >> meta_word >> meta_word;

  // Get file_name;
  if (!(meta_stream >> image_name))
    throw ObjectException(
        "LumenImage did not specify image name (third argument).");

  // Get crops.
  if (!(meta_stream >> extract_x >> extract_y >> extract_w >> extract_h))
    throw ObjectException(
        "LumenImage did not extract rect (forth to seventh arguments).");

  if (meta_stream >> meta_word)
    throw ObjectException("LumenImage got extra argument \"" + meta_word
                          + "\".");

  // Get image.
  auto image_obj = get_object_fn(image_name);
  if (image_obj == nullptr)
    throw ObjectException("LumenImage had failed to get image \"" + image_name
                          + "\".");

  const AssetImage* image = dynamic_cast<const AssetImage*>(image_obj);
  if (image == nullptr)
    throw ObjectException("LumenImage had requested not an image \""
                          + image_name + "\".");

  const auto& image_surf_data = image->surface_data();

  SDL_Surface* cropped_surf = ExtractSurface(image->surface(), extract_x,
                                             extract_y, extract_w, extract_h);
  if (cropped_surf == nullptr)
    throw ObjectException("LumenImage had failed to crop image.");

  // // Set up the cropped surface.
  SDL_SetSurfaceColorKey(cropped_surf, true, image_surf_data.color_key);

  // Extract texture.
  texture_ = SDL_CreateTextureFromSurface(renderer, cropped_surf);
  SDL_DestroySurface(cropped_surf);  // Destroy surface before throwing.

  if (texture_ == nullptr)
    throw ObjectException(
        static_cast<std::string>(
            "LumenImage had failed to create texture due to:\n")
        + SDL_GetError());

  // Set up the extracted texture.
  if (image_surf_data.is_pixel)
    SDL_SetTextureScaleMode(texture_, SDL_SCALEMODE_NEAREST);
}

}  // namespace lumen::object
