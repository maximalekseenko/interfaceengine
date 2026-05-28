// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_RENDER_RENDER_REQUEST_H_
#define SRC_INTERFACE_RENDER_RENDER_REQUEST_H_
#include <cstdint>
#include <string>

#include "interface/dll.h"
#include "interface/render/render_params.h"

namespace interfaceengine::render {

struct INTERFACE_API RenderRequest {
  std::string package_name{""};

  std::string lumen_name{""};

  std::string data{""};

  PosPixel x{0}, y{0}, w{1}, h{1};

  RenderParams params;
};

}  // namespace interfaceengine::render

#endif  // SRC_INTERFACE_RENDER_RENDER_REQUEST_H_
