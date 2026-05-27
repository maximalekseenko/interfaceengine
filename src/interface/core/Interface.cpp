// Copyright 2026 maxim (necromax) alekseenko

#include "interface/core/interface.h"

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

void Interface::SetRootGuiComponent(GuiComponent::Ptr gui_component) {
  impl_->gui_manager_.SetNewRootComponent(std::move(gui_component));

  impl_->UpdateRootComponentSize();
}

void Interface::SendMessageToGui(GuiComponent::Message message,
                                 GuiComponent::Id receiver_id,
                                 bool single_receiver) {
  impl_->gui_manager_.root_component()->DispatchMessage(message, receiver_id,
                                                        single_receiver);
}

void Interface::Quit() { impl_->is_running = false; }

void Interface::AddLogSink(Logger::SinkPtr sink) {
  impl_->logger.AddSink(sink);
}
