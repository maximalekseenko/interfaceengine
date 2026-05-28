// Copyright 2026 maxim (necromax) alekseenko

#include "interface/core/interface.h"

#include <string>
#include <utility>

#include "interface/core/interface_impl.h"
#include "interface/core/settings.h"

Interface::Interface() : impl_{new Impl()} { impl_->self = this; }
Interface::~Interface() { delete impl_; }

void Interface::Initialize() {
  impl_->window_manager.InitSDL();
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

void Interface::SetRootGuiComponent(
    interfaceengine::gui::Component::Ptr gui_component) {
  impl_->gui_manager_.SetNewRootComponent(std::move(gui_component));

  impl_->UpdateRootComponentSize();
}

void Interface::SendMessageToGui(
    interfaceengine::gui::Component::Message message,
    interfaceengine::gui::Component::Id receiver_id, bool single_receiver) {
  impl_->gui_manager_.DispatchMessage(message, receiver_id, single_receiver);
}

void Interface::Quit() { impl_->is_running = false; }

void Interface::AddLogSink(Logger::SinkPtr sink) {
  impl_->logger.AddSink(sink);
}
