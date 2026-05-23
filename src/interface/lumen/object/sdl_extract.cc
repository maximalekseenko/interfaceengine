// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/object/sdl_extract.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include <cstdint>

namespace interfaceengine::lumen::object {

SDL_Surface* ExtractSurface(SDL_Surface* surf, int extract_x, int extract_y,
                            int extract_w, int extract_h) {
  // Check rect being within surface.
  if (extract_x < 0 || extract_y < 0 || extract_x + extract_w > surf->w
      || extract_y + extract_h > surf->h) {
    return nullptr;
  }

  // Crop surface.
  return SDL_CreateSurfaceFrom(
      extract_w, extract_h, surf->format,
      reinterpret_cast<uint8_t*>(surf->pixels) + extract_y * surf->pitch
          + extract_x * SDL_BYTESPERPIXEL(surf->format),
      surf->pitch);
}

}  // namespace interfaceengine::lumen::object
