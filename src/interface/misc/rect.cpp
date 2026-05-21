#include "rect.h"

void GetRect(PosPixel* out_x, PosPixel* out_y, PosPixel* out_w, PosPixel* out_h,
             const PosPixel& parent_x, const PosPixel& parent_y,
             const PosPixel& parent_w, const PosPixel& parent_h,
             const PosPercent& x_percent, const PosPixel& x_offset,
             const PosPercent& y_percent, const PosPixel& y_offset,
             const PosPercent& w_percent, const PosPixel& w_offset,
             const PosPercent& h_percent, const PosPixel& h_offset,
             const HorizontalAlignment& horizontal_alignment,
             const VerticalAlignment& vertical_alignment) {
    PosPixel self_width = parent_w * w_percent + w_offset;
    PosPixel self_height = parent_h * h_percent + h_offset;
    PosPixel self_rel_x = parent_w * x_percent + x_offset;
    PosPixel self_rel_y = parent_h * y_percent + y_offset;
    PosPixel parent_left = parent_x;
    PosPixel parent_right = parent_x + parent_w;
    PosPixel parent_center_x = parent_x + parent_w / 2;
    PosPixel parent_top = parent_y;
    PosPixel parent_bottom = parent_y + parent_h;
    PosPixel parent_center_y = parent_y + parent_h / 2;

    if (out_x != nullptr) {
        switch (horizontal_alignment) {
            case HorizontalAlignment::Left:
                *out_x = parent_left + self_rel_x;
                break;
            case HorizontalAlignment::Right:
                *out_x = parent_right - self_width - self_rel_x;
                break;
            case HorizontalAlignment::Center:
                *out_x = parent_center_x - self_width / 2 - self_rel_x;
                break;
            default:
                *out_x = 0;
                break;
        }
    }

    if (out_y != nullptr) {
        switch (vertical_alignment) {
            case VerticalAlignment::Top:
                *out_y = parent_top + self_rel_y;
                break;
            case VerticalAlignment::Bottom:
                *out_y = parent_bottom - self_height - self_rel_y;
                break;
            case VerticalAlignment::Center:
                *out_y = parent_center_y - self_height / 2 - self_rel_y;
                break;
            default:
                *out_y = 0;
                break;
        }
    }

    if (out_w != nullptr) *out_w = self_width;

    if (out_h != nullptr) *out_h = self_height;
}

bool IsWithinRect(const PosPixel& x, const PosPixel& y, const PosPixel& rect_x,
                  const PosPixel& rect_y, const PosPixel& rect_w,
                  const PosPixel& rect_h) {
    return (x >= rect_x) && (x < rect_x + rect_w) && (y >= rect_y)
           && (y < rect_y + rect_h);
}