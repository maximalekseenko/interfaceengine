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

  RelScalar x{0, 0}, y{0, 0}, w{1, 0}, h{1, 0};

  HorizontalAlignment horizontal_alignment{HorizontalAlignment::Left};
  VerticalAlignment vertical_alignment{VerticalAlignment::Top};

  render::RenderParams params;
};

}  // namespace interfaceengine::gui

#endif  // INCLUDE_INTERFACE_GUI_LUMEN_RULES_H_
