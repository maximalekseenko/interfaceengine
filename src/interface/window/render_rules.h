// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_WINDOW_RENDER_RULES_H_
#define SRC_INTERFACE_WINDOW_RENDER_RULES_H_

#include <string>

#include "interface/dll.h"
#include "interface/position_types.h"

namespace interfaceengine::window {

/// @brief A structure that holds information
/// for rendering a texture on the window.
struct INTERFACE_INTERNAL RenderRules {
  /// @brief Position of top left corner of the texture on the screen to be
  /// rendered at.
  ///
  /// Measured in pixels from top left corner of the screen.
  PosPixel x = 0, y = 0;

  /// @brief Scale of a texture to be rendered on the screen.
  ///
  /// Will use default scale for texture if set to zero.
  PosPixel w = 0, h = 0;

  /// @brief Offset of a pivot point for rotating the texture.
  ///
  /// Measured in pixels from top left corner of the texture.
  PosPixel rot_x_offset = 0, rot_y_offset = 0;

  /// @brief Position of a pivot point for rotating the texture.
  ///
  /// Measured in scales from 0 to 1.
  PosPercent rot_x_percent = 0.5, rot_y_percent = 0.5;

  /// @brief Angle of rotation in degrees.
  PosAngle rotation_angle = 0;

  HorizontalAlignment horizontal_anchor{HorizontalAlignment::Left};
  VerticalAlignment vertical_anchor{VerticalAlignment::Top};

  /// @brief TODO remove
  PosPixel crop_x = 0, crop_y = 0, crop_w = 0, crop_h = 0;
};

}  // namespace interfaceengine::window

#endif  // SRC_INTERFACE_WINDOW_RENDER_RULES_H_
