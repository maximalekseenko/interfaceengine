// Copyright 2026 maxim (necromax) alekseenko

#include "interface/window/manager.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "interface/window/exceptions.h"

namespace interfaceengine::window {

void WindowManager::InitSDL() {
  if (!SDL_Init(SDL_INIT_VIDEO)) return;
  // impl_->logger.Log("SDL had failed to initialize", LogLevel::Error);
  if (!TTF_Init()) return;  // TODO(necromax): error!
  // impl_->logger.Log("SDL_TTF had failed to initialize", LogLevel::Error);
}

void WindowManager::CreateWindow(int32_t width, int32_t height,
                                 uint64_t flags) {
  if (sdl_window_ != nullptr)
    throw InitializationError("self", "WindowManager is already initialized.");

  flags |= SDL_WINDOW_RESIZABLE;

  // TODO(necromax): title as a variable?
  sdl_window_ = SDL_CreateWindow("battlefleet", width, height, flags);

  if (sdl_window_ == nullptr)
    throw InitializationError("window", SDL_GetError());

  sdl_renderer_ = SDL_CreateRenderer(sdl_window_, NULL);

  if (sdl_window_ == nullptr)
    throw InitializationError("renderer", SDL_GetError());

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

  if (!render_success) throw RenderError(SDL_GetError());
}

void WindowManager::GetWindowSize(int* out_w, int* out_h) {
  SDL_GetWindowSize(sdl_window_, out_w, out_h);
}

}  // namespace interfaceengine::window
