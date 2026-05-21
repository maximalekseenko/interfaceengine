#pragma once

#include <cstdint>

#include "dll.h"
#include "position_types.h"

struct DLL_PUBLIC MouseOverEvent {
    PosPixel x, y;
};

struct DLL_PUBLIC MouseClickEvent {
    bool is_down;
    uint8_t button;
    PosPixel x, y;
};