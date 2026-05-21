// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_SDL_EXTRACT_H_
#define SRC_INTERFACE_LUMEN_OBJECT_SDL_EXTRACT_H_

#include <SDL3/SDL_surface.h>

#include <cstdint>

#include "interface/dll.h"

namespace lumen::object {

DLL_LOCAL SDL_Surface* ExtractSurface(SDL_Surface* surf, uint32_t extract_x,
                                      uint32_t extract_y, uint32_t extract_w,
                                      uint32_t extract_h);

}  // namespace lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_SDL_EXTRACT_H_
