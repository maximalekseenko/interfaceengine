// Copyright 2026 maxim (necromax) alekseenko

#include "interface/core/interface.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <necroutils/colexc.h>
#include <necroutils/logger.h>

#include <memory>
#include <stack>
#include <string>
#include <utility>

#include "interface/core/interface_impl.h"
#include "interface/core/settings.h"
#include "interface/lumen/manager/manager.h"
#include "interface/misc/rect.h"
#include "interface/window/manager.h"

Interface::Interface() : impl_{new Impl()} { impl_->self = this; }
Interface::~Interface() { delete impl_; }

void Interface::Initialize() {
  // Initialize SDL.
  if (!SDL_Init(SDL_INIT_VIDEO))
    impl_->logger.Log("SDL had failed to initialize", LogLevel::Error);
  if (!TTF_Init())
    impl_->logger.Log("SDL_TTF had failed to initialize", LogLevel::Error);

  impl_->window_manager.CreateWindow();
  impl_->settings.executable_path = SDL_GetBasePath();

  impl_->logger.Log("Interface initialized.", LogLevel::Debug);
}

void Interface::Run() {
  impl_->is_running = true;
  while (impl_->is_running) {
    impl_->HandleEvents();
    impl_->Render();
    impl_->TickDelay();
  }
}

void Interface::SetRootGuiComponent(GuiComponent::Ptr gui_component) {
  impl_->root_gui_component = std::move(gui_component);

  impl_->UpdateRootComponentSize();
}

void Interface::SendMessageToGui(GuiComponent::Message message,
                                 GuiComponent::Id receiver_id,
                                 bool single_receiver) {
  impl_->root_gui_component->DispatchMessage(message, receiver_id,
                                             single_receiver);
}

void Interface::Quit() { impl_->is_running = false; }

void Interface::AddLogSink(Logger::SinkPtr sink) {
  impl_->logger.AddSink(sink);
}
