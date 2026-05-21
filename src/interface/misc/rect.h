#pragma once

#include "interface/dll.h"
#include "interface/position_types.h"

DLL_LOCAL void GetRect(PosPixel* out_x, PosPixel* out_y, PosPixel* out_w,
                       PosPixel* out_h, const PosPixel& parent_x,
                       const PosPixel& parent_y, const PosPixel& parent_w,
                       const PosPixel& parent_h, const PosPercent& x_percent,
                       const PosPixel& x_offset, const PosPercent& y_percent,
                       const PosPixel& y_offset, const PosPercent& w_percent,
                       const PosPixel& w_offset, const PosPercent& h_percent,
                       const PosPixel& h_offset,
                       const HorizontalAlignment& horizontal_alignment,
                       const VerticalAlignment& vertical_alignment);

DLL_LOCAL bool IsWithinRect(const PosPixel& x, const PosPixel& y,
                            const PosPixel& rect_x, const PosPixel& rect_y,
                            const PosPixel& rect_w, const PosPixel& rect_h);