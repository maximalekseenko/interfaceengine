// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_
#define INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_

#include <string>

#include "interface/dll.h"
#include "interface/position_types.h"
#include "interface/render/render_params.h"

namespace interfaceengine::gui {

struct INTERFACE_API LumenRules {
  std::string package_name{""};

  std::string lumen_name{""};

  std::string data{""};

  PosPercent x_percent{0}, y_percent{0}, w_percent{1}, h_percent{1};

  PosPixel x_offset{0}, y_offset{0}, w_offset{0}, h_offset{0};
  HorizontalAlignment horizontal_alignment{HorizontalAlignment::Left};
  VerticalAlignment vertical_alignment{VerticalAlignment::Top};

  render::RenderParams params;
};

}  // namespace interfaceengine::gui

#endif  // INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_
