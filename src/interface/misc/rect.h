// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_MISC_RECT_H_
#define SRC_INTERFACE_MISC_RECT_H_

#include "interface/dll.h"
#include "interface/position_types.h"

INTERFACE_INTERNAL void GetRect(PosPixel* out_x, PosPixel* out_y,          //
                                PosPixel* out_w, PosPixel* out_h,          //
                                PosPixel parent_x, PosPixel parent_y,      //
                                PosPixel parent_w, PosPixel parent_h,      //
                                RelScalar self_x, RelScalar self_y,        //
                                RelScalar self_w, RelScalar self_h,        //
                                HorizontalAlignment horizontal_alignment,  //
                                VerticalAlignment vertical_alignment);

INTERFACE_INTERNAL bool IsWithinRect(PosPixel x, PosPixel y, PosPixel rect_x,
                                     PosPixel rect_y, PosPixel rect_w,
                                     PosPixel rect_h);

#endif  // SRC_INTERFACE_MISC_RECT_H_
