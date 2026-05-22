// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_ASSET_H_
#define SRC_INTERFACE_LUMEN_OBJECT_ASSET_H_

#include <istream>
#include <string>

#include "interface/dll.h"
#include "interface/lumen/object/object.h"

namespace interfaceengine::lumen::object {

class INTERFACE_INTERNAL Asset : public Object {
 public:
  virtual ~Asset() = default;
};

}  // namespace interfaceengine::lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_ASSET_H_
