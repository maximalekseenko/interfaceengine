// Copyright 2026 maxim (necromax) alekseenko

#include <utility>
#include <vector>

#include "interface/gui/component.h"
#include "interface/misc/rect.h"

namespace interfaceengine::gui {

std::vector<LumenRules> Component::GetLumenRules() { return {}; }

void Component::SetId(Id id) { id_ = id; }

Component::Id Component::GetId() { return id_; }

void Component::AddChildComponent(Ptr child_ptr) {
  child_ptr->parent = this;
  this->children.push_back(std::move(child_ptr));
}

void Component::SetWidth(PosPercent const& percent, PosPixel const& offset) {
  this->w_percent = percent;
  this->w_offset = offset;
}

void Component::SetHorizontalPosition(PosPercent const& percent,
                                      PosPixel const& offset,
                                      HorizontalAlignment const& alignment) {
  this->x_percent = percent;
  this->x_offset = offset;
  this->horizontal_alignment = alignment;
}

void Component::SetHeight(PosPercent const& percent, PosPixel const& offset) {
  this->h_percent = percent;
  this->h_offset = offset;
}

void Component::SetVerticalPosition(PosPercent const& percent,
                                    PosPixel const& offset,
                                    VerticalAlignment const& alignment) {
  this->y_percent = percent;
  this->y_offset = offset;
  this->vertical_alignment = alignment;
}

void Component::GetSelfRect(PosPixel* out_x, PosPixel* out_y, PosPixel* out_w,
                            PosPixel* out_h) const {
  PosPixel parent_x, parent_y, parent_w, parent_h;
  if (this->parent != nullptr) {
    this->parent->GetSelfRect(&parent_x, &parent_y, &parent_w, &parent_h);
  } else {
    parent_x = 0;
    parent_y = 0;
    parent_w = 0;
    parent_h = 0;
  }

  GetRect(out_x, out_y, out_w, out_h, parent_x, parent_y, parent_w, parent_h,
          x_percent, x_offset, y_percent, y_offset, w_percent, w_offset,
          h_percent, h_offset, horizontal_alignment, vertical_alignment);
}

void Component::DispatchMessage(Message message, Id receiver_id,
                                bool single_receiver) {
  if (GetId() == receiver_id) {
    on_message(message);
    if (single_receiver) return;
  }

  for (auto& child : children)
    child->DispatchMessage(message, receiver_id, single_receiver);
}

void Component::DispatchMouseOver(MouseEvent event) {
  PosPixel self_x, self_y, self_w, self_h;
  GetSelfRect(&self_x, &self_y, &self_w, &self_h);
  if (!IsWithinRect(event.x, event.y, self_x, self_y, self_w, self_h)) return;

  if (on_mouse_over) on_mouse_over(event);
  for (auto& child : children) child->DispatchMouseOver(event);
}

void Component::DispatchMouseClick(MouseEvent event) {
  PosPixel self_x, self_y, self_w, self_h;
  GetSelfRect(&self_x, &self_y, &self_w, &self_h);
  if (!IsWithinRect(event.x, event.y, self_x, self_y, self_w, self_h)) return;

  if (on_mouse_click) on_mouse_click(event);
  for (auto& child : children) child->DispatchMouseClick(event);
}

}  // namespace interfaceengine::gui
