// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_RENDER_RENDER_PARAMS_H_
#define INCLUDE_INTERFACE_RENDER_RENDER_PARAMS_H_

#include <cstdint>

#include "interface/dll.h"
#include "interface/position_types.h"

namespace interfaceengine::render {

struct INTERFACE_API RenderParams {
  HorizontalAlignment horizontal_anchor{HorizontalAlignment::Left};
  VerticalAlignment vertical_anchor{VerticalAlignment::Top};

  uint16_t color_mod;

  PosAngle rotation_angle{0};
  PosPercent rot_x_percent{0.5}, rot_y_percent{0.5};
  PosPixel rot_x_offset{0}, rot_y_offset{0};
};

}  // namespace interfaceengine::render

#endif  // INCLUDE_INTERFACE_RENDER_RENDER_PARAMS_H_
