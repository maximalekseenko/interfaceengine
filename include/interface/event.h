// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_EVENT_H_
#define INCLUDE_INTERFACE_EVENT_H_

#include <cstdint>

#include "interface/dll.h"
#include "interface/position_types.h"

struct DLL_PUBLIC MouseOverEvent {
  PosPixel x, y;
};

struct DLL_PUBLIC MouseClickEvent {
  bool is_down;
  uint8_t button;
  PosPixel x, y;
};

#endif  // INCLUDE_INTERFACE_EVENT_H_
