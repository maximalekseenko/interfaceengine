// Copyright 2026 maxim (necromax) alekseenko

#include "interface/gui/component.h"

#include <utility>

#include "interface/misc/rect.h"

namespace interfaceengine::gui {

void Component::AddChildComponent(Ptr child_ptr) {
  child_ptr->parent_ = this;
  children_.push_back(std::move(child_ptr));
}

void Component::GetSelfRect(PosPixel* out_x, PosPixel* out_y, PosPixel* out_w,
                            PosPixel* out_h) const {
  PosPixel parent_x, parent_y, parent_w, parent_h;
  if (parent_ != nullptr) {
    parent_->GetSelfRect(&parent_x, &parent_y, &parent_w, &parent_h);
  } else {
    parent_x = 0;
    parent_y = 0;
    parent_w = 0;
    parent_h = 0;
  }

  GetRect(out_x, out_y, out_w, out_h, parent_x, parent_y, parent_w, parent_h,
          x_, y_, w_, h_, horizontal_alignment_, vertical_alignment_);
}

bool Component::IsPointWithin(PosPixel x, PosPixel y) {
  PosPixel self_x, self_y, self_w, self_h;
  GetSelfRect(&self_x, &self_y, &self_w, &self_h);
  return IsWithinRect(x, y, self_x, self_y, self_w, self_h);
}

}  // namespace interfaceengine::gui
