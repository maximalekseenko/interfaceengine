// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_EVENT_H_
#define INCLUDE_INTERFACE_EVENT_H_

#include <cstdint>

#include "interface/dll.h"
#include "interface/position_types.h"

struct INTERFACE_API MouseEvent {
  bool is_down{false};
  uint8_t button{0};
  PosPixel x{0}, y{0};
};

#endif  // INCLUDE_INTERFACE_EVENT_H_
