// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_WINDOW_RENDER_COMMAND_H_
#define SRC_INTERFACE_WINDOW_RENDER_COMMAND_H_

#include <SDL3/SDL_render.h>

#include <cstdint>
#include <string>

#include "interface/dll.h"
#include "interface/render/render_params.h"

namespace interfaceengine::window {

struct INTERFACE_API RenderCommand {
  PosPixel x{0}, y{0}, w{1}, h{1};

  SDL_Texture* texture;

  render::RenderParams params;
};

}  // namespace interfaceengine::window

#endif  // SRC_INTERFACE_WINDOW_RENDER_COMMAND_H_
