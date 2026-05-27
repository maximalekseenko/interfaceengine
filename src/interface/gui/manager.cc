// Copyright 2026 maxim (necromax) alekseenko

#include "interface/gui/manager.h"

#include <utility>

#include "interface/gui/component.h"

namespace interfaceengine::gui {

void Manager::SetNewRootComponent(Component::Ptr new_root_component) {
  root_component_ = std::move(new_root_component);
}

void Manager::UpdateForWindowSize(int window_w, int window_h) {
  root_component_->SetWidth(0, window_w);
  root_component_->SetHeight(0, window_h);
}

}  // namespace interfaceengine::gui
