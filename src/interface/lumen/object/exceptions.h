// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_EXCEPTIONS_H_
#define SRC_INTERFACE_LUMEN_OBJECT_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

#include "interface/dll.h"

namespace lumen::object {

class DLL_LOCAL ObjectException : public std::runtime_error {
 public:
  explicit ObjectException(const std::string& message)
      : std::runtime_error(message) {}
};

}  // namespace lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_EXCEPTIONS_H_
