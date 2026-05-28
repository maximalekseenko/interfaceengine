// Copyright 2026 maxim (necromax) alekseenko

#include "interface/core/interface_impl.h"

#include <SDL3/SDL.h>
#include <necroutils/colexc.h>
#include <necroutils/logger.h>

#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "interface/core/settings.h"
#include "interface/lumen/manager/manager.h"
#include "interface/misc/rect.h"
#include "interface/render/render_request.h"
#include "interface/window/manager.h"
#include "interface/window/render_command.h"

Interface::Impl::Impl() : logger("InterfaceEngine") {}

void Interface::Impl::Render() {
  // Clear past.
  window_manager.ClearRender();

  std::vector<interfaceengine::render::RenderRequest> requests;
  try {
    gui_manager_.GatherRenderRequests(requests);
    for (auto request : requests) {
      window_manager.RenderTexture(
          {.x = request.x,
           .y = request.y,
           .w = request.w,
           .h = request.h,
           .texture = this->lumen_manager.GetLumenTexture(
               request.package_name, request.lumen_name, request.data),
           .params = request.params});
    }
  } catch (const std::exception& e) {
    logger.Log(std::string("Had failed a render cycle due to:\n") + e.what(),
               LogLevel::Error);
  }

  // Apply rendering.
  window_manager.ApplyRender();
}

void Interface::Impl::TickDelay() {
  // Get tick of current frame.
  auto curr_frame_tick = SDL_GetTicks();

  // Get frame duration.
  auto ms_per_frame = 1000 / this->settings.fps;
  auto curr_frame_duration = curr_frame_tick - this->last_frame_tick;

  // Wait if frame rendering was too fast.
  if (ms_per_frame > curr_frame_duration)
    SDL_Delay(ms_per_frame - curr_frame_duration);

  // Save current frame
  this->last_frame_tick = curr_frame_tick;
}

void Interface::Impl::HandleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) this->self->Quit();
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
      gui_manager_.DispatchMouseClick({.is_down = true,
                                       .button = event.button.button,
                                       .x = event.button.x,
                                       .y = event.button.y});
    if (event.type == SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED)
      UpdateRootComponentSize();
  }
}

void Interface::LoadLumens(std::string package_path) {
  SDL_Renderer* renderer = impl_->window_manager.renderer();

  try {
    impl_->lumen_manager.LoadLumPackage(package_path, renderer);
  } catch (const std::exception& e) {
    impl_->logger.Log(std::string("Failed to load lum package:") + e.what(),
                      LogLevel::Error);
  } catch (...) {
    impl_->logger.Log("Interface::LoadLumens",
                      LogLevel::Error);  // TODO(necromax): fix all thisd
                                         // sdifhsdfiouhfsdiuasdfhgSDO!
  }
}

void Interface::Impl::UpdateRootComponentSize() {
  int window_w, window_h;
  window_manager.GetWindowSize(&window_w, &window_h);
  gui_manager_.UpdateForWindowSize(window_w, window_h);
}
