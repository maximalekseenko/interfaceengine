/// @file window_manager.cpp
/// @author maxim (necromax) alekseenko
/// @brief This file contains definition of WindowManager.

#pragma once

#include <SDL3/SDL.h>
#include <necroutils/logger.h>

#include <cstdint>

#include "interface/dll.h"
#include "render_rules.h"

class DLL_LOCAL WindowManager {
 public:  // -------------------- CONSTRUCTOR --------------------
  WindowManager(std::shared_ptr<Logger> logger);

 public:  // -------------------- PUBLIC METHODS --------------------
  SDL_Renderer* GetRenderer();

  /// @brief Creates a window of set size.
  /// @param width The width of the window created.
  /// @param height The height of the window.
  /// @param flags Flags for sdl window.
  void CreateWindow(int32_t width = 640, int32_t height = 480,
                    uint64_t flags = 0);

  /// @brief Clears render for the begining of the rendering cycle.
  void ClearRender();

  /// @brief Applies render for the end of the rendering cycle.
  void ApplyRender();

  /// @brief Renders texture on the screen.
  /// @param texture SDL texture to render.
  void RenderTexture(SDL_Texture* texture, const RenderRules& render_rules);

  /// @brief Returns current window size.
  /// @param[out] out_w Current width of the window.
  /// @param[out] out_h Current height of the window.
  void GetWindowSize(int* out_w, int* out_h);

 private:  // -------------------- PRIVATE MEMBERS --------------------
  /// @brief SDL window.
  SDL_Window* sdl_window{nullptr};

  /// @brief SDL renderer.
  SDL_Renderer* sdl_renderer{nullptr};

  /// @brief Interface's logger.
  std::shared_ptr<Logger> logger;
};