// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/object/asset_image.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include <sstream>
#include <string>
#include <vector>

#include "interface/lumen/object/exceptions.h"
#include "interface/lumen/object/sdl_io.h"

namespace lumen::object {

AssetImage::~AssetImage() {
  if (surface_ != nullptr) SDL_DestroySurface(surface_);
}
void AssetImage::Load(const std::string& meta, const getFileFn& get_file_fn,
                      const getObjectFn& get_object_fn,
                      SDL_Renderer* renderer) {
  std::stringstream meta_stream(meta);
  std::string meta_word;
  std::string file_name;

  // Skip type and name.
  meta_stream >> meta_word >> meta_word;

  // Get file_name;
  if (!(meta_stream >> file_name))
    throw ObjectException(
        "AssetImage did not specify image file name (third argument).");

  // Parse data.
  while (meta_stream >> meta_word) {
    // Set pixel rule.
    if (meta_word == "pixel") {
      if (!(meta_stream >> surface_data_.is_pixel))
        continue;  // TODO(necromax): log

    } else if (meta_word == "colorkey") {
      if (!(meta_stream >> std::hex >> surface_data_.color_key))
        continue;  // TODO(necromax): log

    } else {
      throw ObjectException("AssetImage got an unknown argument \"" + meta_word
                            + "\".");
    }
  }

  // Try making image.
  auto file_io = get_file_fn(file_name);

  if (file_io == nullptr)
    throw ObjectException("AssetImage failed to get file \"" + file_name
                          + "\".");

  SDL_IOStream* sdl_io = OpenIOFromIStream(*file_io);

  if (!sdl_io)
    throw ObjectException("AssetImage failed to make stream from file \""
                          + file_name + "\" due to:\n" + SDL_GetError());

  surface_ = IMG_Load_IO(sdl_io, true);

  if (surface_ == nullptr)
    throw ObjectException("AssetImage failed to make load image from file \""
                          + file_name + "\" due to:\n" + SDL_GetError());
}

}  // namespace lumen::object
