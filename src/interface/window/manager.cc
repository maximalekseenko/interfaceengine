// Copyright 2026 maxim (necromax) alekseenko

#include "interface/window/manager.h"

#include <string>

namespace interfaceengine::window {

void WindowManager::CreateWindow(int32_t width, int32_t height,
                                 uint64_t flags) {
  if (sdl_window_ != nullptr)
    throw std::runtime_error(
        "Trying to create a window when once already exits");

  flags |= SDL_WINDOW_RESIZABLE;

  // TODO(necromax): title as a variable?
  // TODO(necromax): exceptions?
  sdl_window_ = SDL_CreateWindow("battlefleet", width, height, flags);

  if (sdl_window_ == nullptr)
    throw std::runtime_error("Had failed to create a window.");

  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, NULL);

  if (sdl_window_ == nullptr)
    throw std::runtime_error("Had failed to create a renderer.");

  SDL_SetRenderDrawBlendMode(sdl_renderer_, SDL_BLENDMODE_BLEND);
}

void WindowManager::ClearRender() { SDL_RenderClear(sdl_renderer_); }

void WindowManager::ApplyRender() { SDL_RenderPresent(sdl_renderer_); }

void WindowManager::RenderTexture(struct SDL_Texture* texture,
                                  const RenderRules& renderRules) {
  SDL_FRect dst = {.x = renderRules.x,
                   .y = renderRules.y,
                   .w = renderRules.w,
                   .h = renderRules.h};

  // Fix width and height for lumen aspect-based defines.
  if (renderRules.w == 0 || renderRules.h == 0) {
    float texture_w, texture_h;
    SDL_GetTextureSize(texture, &texture_w, &texture_h);

    if (renderRules.w == 0 && renderRules.h == 0) {
      dst.w = texture_w;
      dst.h = texture_h;
    } else if (renderRules.w != 0) {
      dst.w = renderRules.w;
      dst.h = static_cast<float>(renderRules.w) * texture_h / texture_w;
    } else {  // renderRules.h != 0
      dst.w = static_cast<float>(renderRules.h) * texture_w / texture_h;
      dst.h = texture_h;
    }
  }

  // Fix for anchors.
  if (renderRules.horizontal_anchor == HorizontalAlignment::Center)
    dst.x -= dst.w / 2;
  else if (renderRules.horizontal_anchor == HorizontalAlignment::Right)
    dst.x -= dst.w;
  if (renderRules.vertical_anchor == VerticalAlignment::Center)
    dst.y -= dst.h / 2;
  else if (renderRules.vertical_anchor == VerticalAlignment::Bottom)
    dst.y -= dst.h;

  // Render texture.
  constexpr float kRotationEpsilon = 0.001f;  // TODO(necromax) maybe parameter?
  bool render_success;
  if (std::abs(renderRules.rotation_angle) < kRotationEpsilon) {
    render_success = SDL_RenderTexture(sdl_renderer_, texture, nullptr, &dst);
  } else {
    SDL_FPoint rotation_center{
        .x = dst.w * renderRules.rot_x_percent + renderRules.rot_x_offset,
        .y = dst.h * renderRules.rot_y_percent + renderRules.rot_y_offset};
    render_success = SDL_RenderTextureRotated(sdl_renderer_, texture, nullptr,
                                              &dst, renderRules.rotation_angle,
                                              &rotation_center, SDL_FLIP_NONE);
  }

  if (!render_success)
    throw std::runtime_error(std::string("Had failed to render:\n")
                             + SDL_GetError());
}

void WindowManager::GetWindowSize(int* out_w, int* out_h) {
  SDL_GetWindowSize(sdl_window_, out_w, out_h);
}

}  // namespace interfaceengine::window
