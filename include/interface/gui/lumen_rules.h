// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_
#define INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_
#include <cstdint>
#include <string>

#include "interface/dll.h"
#include "interface/position_types.h"

/// @brief Composition of rules for rendering lumens for gui component.
struct INTERFACE_API LumenRules {
  /// @brief Name of a package of the lumen this rule refers to.
  std::string package_name{""};

  /// @brief Name of a lumen this rule refers to.
  std::string lumen_name{""};

  PosPercent x_percent{0}, y_percent{0}, w_percent{1}, h_percent{1};

  PosPixel x_offset{0}, y_offset{0}, w_offset{0}, h_offset{0};

  HorizontalAlignment horizontal_alignment{HorizontalAlignment::Left};
  VerticalAlignment vertical_alignment{VerticalAlignment::Top};

  HorizontalAlignment horizontal_anchor{HorizontalAlignment::Left};
  VerticalAlignment vertical_anchor{VerticalAlignment::Top};

  /// @brief Color modification of the lumen.
  /// Measured in rgb encoding.
  uint16_t color_mod;

  /// @brief Rotation of the limen.
  /// Measured in degrees.
  PosAngle rotation_angle{0};
  PosPercent rot_x_percent{0.5}, rot_y_percent{0.5};
  PosPixel rot_x_offset{0}, rot_y_offset{0};

  /// @brief Additional data for the lumen.
  /// Used for special types of lumens (i.e. TextLumen).
  std::string data;
};

#endif  // INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_
