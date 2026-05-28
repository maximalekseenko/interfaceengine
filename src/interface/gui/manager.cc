// Copyright 2026 maxim (necromax) alekseenko

#include "interface/gui/manager.h"

#include <necroutils/colexc.h>

#include <stack>
#include <utility>
#include <vector>

#include "interface/gui/component.h"
#include "interface/gui/lumen_rules.h"
#include "interface/misc/rect.h"

namespace interfaceengine::gui {

void GuiManager::SetNewRootComponent(Component::Ptr new_root_component) {
  root_component_ = std::move(new_root_component);
}

void GuiManager::DispatchMessage(Component::Message message,
                                 Component::Id receiver_id,
                                 bool single_receiver) {
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

    // Add children to queue.
    for (auto& component_child : component->children_)
      component_queue.push(component_child.get());
  }
}

void GuiManager::DispatchMouseOver(const MouseEvent& event) {
  std::stack<Component*> component_queue;
  component_queue.push(root_component_.get());

  while (!component_queue.empty()) {
    Component* component = component_queue.top();
    component_queue.pop();

    // Execute event on this component.
    if (component->IsPointWithin(event.x, event.y))
      component->OnMouseOver(event);

    // Add children to queue.
    for (auto& component_child : component->children_)
      component_queue.push(component_child.get());
  }
}

void GuiManager::DispatchMouseClick(const MouseEvent& event) {
  std::stack<Component*> component_queue;
  component_queue.push(root_component_.get());

  while (!component_queue.empty()) {
    Component* component = component_queue.top();
    component_queue.pop();

    // Execute event on this component.
    if (component->IsPointWithin(event.x, event.y))
      component->OnMouseClick(event);

    // Add children to queue.
    for (auto& component_child : component->children_)
      component_queue.push(component_child.get());
  }
}

void GuiManager::GatherRenderRequests(
    std::vector<render::RenderRequest>& out_requests) {
  std::stack<Component*> component_queue;
  component_queue.push(root_component_.get());

  while (!component_queue.empty()) {
    Component* component = component_queue.top();
    component_queue.pop();

    // Gather lumen rules.
    auto component_rules = component->GetLumenRules();

    for (auto lumen_rule : component_rules) {
      render::RenderRequest new_render_request{
          .package_name = lumen_rule.package_name,
          .lumen_name = lumen_rule.lumen_name,
          .data = lumen_rule.data,
          .params = lumen_rule.params,
      };

      PosPixel component_x, component_y, component_w, component_h;
      component->GetSelfRect(&component_x, &component_y, &component_w,
                             &component_h);

      GetRect(&new_render_request.x, &new_render_request.y,            //
              &new_render_request.w, &new_render_request.h,            //
              component_x, component_y, component_w, component_h,      //
              lumen_rule.x, lumen_rule.y, lumen_rule.w, lumen_rule.h,  //
              lumen_rule.horizontal_alignment, lumen_rule.vertical_alignment);

      out_requests.push_back(new_render_request);
    }

    // Add children to queue.
    for (auto& component_child : component->children_)
      component_queue.push(component_child.get());
  }
}

void GuiManager::UpdateForWindowSize(int window_w, int window_h) {
  root_component_->SetWidth({0, static_cast<PosPixel>(window_w)});
  root_component_->SetHeight({0, static_cast<PosPixel>(window_h)});
}

}  // namespace interfaceengine::gui
