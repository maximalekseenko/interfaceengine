// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_TYPES_H_
#define SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_TYPES_H_

#include <cstdint>

namespace interfaceengine::lumen::package {

using Magic = char[4];
using Version = uint32_t;
using ObjectCount = uint32_t;
using MetaSize = uint32_t;
using ObjectSize = uint32_t;

}  // namespace interfaceengine::lumen::package

#endif  // SRC_INTERFACE_LUMEN_PACKAGE_PACKAGE_TYPES_H_
