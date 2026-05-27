// Copyright 2026 maxim (necromax) alekseenko

#include "interface/gui/manager.h"

#include <stack>
#include <utility>

#include "interface/gui/component.h"

namespace interfaceengine::gui {

void Manager::SetNewRootComponent(Component::Ptr new_root_component) {
  root_component_ = std::move(new_root_component);
}

void Manager::DispatchMessage(Component::Message message,
                              Component::Id receiver_id, bool single_receiver) {
  std::stack<Component*> component_queue;
  component_queue.push(root_component_.get());

  while (!component_queue.empty()) {
    Component* component = component_queue.top();
    component_queue.pop();

    // Execute event on this component.
    if (component->id() == receiver_id) {
      component->OnMessage(message);
      if (single_receiver) break;
    }

    // Add children to stack.
    for (auto& component_child : component->children)
      component_queue.push(component_child.get());
  }
}

void Manager::DispatchMoveOver(const MouseEvent& event) {
  std::stack<Component*> component_queue;
  component_queue.push(root_component_.get());

  while (!component_queue.empty()) {
    Component* component = component_queue.top();
    component_queue.pop();

    // Execute event on this component.
    component->OnMouseOver(event);

    // Add children to stack.
    for (auto& component_child : component->children)
      component_queue.push(component_child.get());
  }
}

void Manager::DispatchMoveClick(const MouseEvent& event) {
  std::stack<Component*> component_queue;
  component_queue.push(root_component_.get());

  while (!component_queue.empty()) {
    Component* component = component_queue.top();
    component_queue.pop();

    // Execute event on this component.
    component->OnMouseClick(event);

    // Add children to stack.
    for (auto& component_child : component->children)
      component_queue.push(component_child.get());
  }
}

void Manager::UpdateForWindowSize(int window_w, int window_h) {
  root_component_->SetWidth(0, window_w);
  root_component_->SetHeight(0, window_h);
}

}  // namespace interfaceengine::gui
