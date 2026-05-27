// Copyright 2026 maxim (necromax) alekseenko

#include "interface/gui/component.h"

#include <utility>
#include <vector>

#include "interface/misc/rect.h"

namespace interfaceengine::gui {

void Component::AddChildComponent(Ptr child_ptr) {
  child_ptr->parent = this;
  this->children.push_back(std::move(child_ptr));
}

void Component::SetWidth(PosPercent const& percent, PosPixel const& offset) {
  this->w_percent_ = percent;
  this->w_offset_ = offset;
}

void Component::SetHorizontalPosition(PosPercent const& percent,
                                      PosPixel const& offset,
                                      HorizontalAlignment const& alignment) {
  this->x_percent_ = percent;
  this->x_offset_ = offset;
  this->horizontal_alignment_ = alignment;
}

void Component::SetHeight(PosPercent const& percent, PosPixel const& offset) {
  this->h_percent_ = percent;
  this->h_offset_ = offset;
}

void Component::SetVerticalPosition(PosPercent const& percent,
                                    PosPixel const& offset,
                                    VerticalAlignment const& alignment) {
  this->y_percent_ = percent;
  this->y_offset_ = offset;
  this->vertical_alignment_ = alignment;
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
          x_percent_, x_offset_, y_percent_, y_offset_, w_percent_, w_offset_,
          h_percent_, h_offset_, horizontal_alignment_, vertical_alignment_);
}

}  // namespace interfaceengine::gui
