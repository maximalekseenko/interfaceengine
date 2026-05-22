// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_HEADER_H_
#define SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_HEADER_H_

#include <array>
#include <cstdint>
#include <string>

#include "interface/dll.h"

namespace interfaceengine::lumen::package {

struct INTERFACE_INTERNAL PackageHeader {
  using Magic = std::array<char, 4>;
  using MetaSize = std::uint32_t;
  using FileSize = std::uint64_t;
  using FileNameSize = std::uint32_t;
  using ElemCount = std::uint64_t;
  using Version = std::uint32_t;

  Magic magic = {'L', 'U', 'M', 'X'};
  Version version = static_cast<Version>(-1);
  ElemCount files_count = 0;
  ElemCount meta_lines_count = 0;
};

}  // namespace interfaceengine::lumen::package

#endif  // SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_HEADER_H_
