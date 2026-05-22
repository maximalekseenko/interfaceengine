// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_WINDOW_EXCEPTIONS_H_
#define SRC_INTERFACE_WINDOW_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

#include "interface/dll.h"

namespace interfaceengine::window {

class DLL_LOCAL ManagerException : public std::runtime_error {
 public:
  explicit ManagerException(const std::string& message)
      : std::runtime_error(message) {}
};

class DLL_LOCAL InitializationError : public ManagerException {
 public:
  explicit InitializationError(const std::string& subject,
                               const std::string& reason)
      : ManagerException("WindowManager had failed to initialize " + subject
                         + " due to:\n" + reason) {}
};

class DLL_LOCAL RenderError : public ManagerException {
 public:
  explicit RenderError(const std::string& reason)
      : ManagerException("WindowManager had failed to render a lumen due to:\n"
                         + reason) {}
};

}  // namespace interfaceengine::window

#endif  // SRC_INTERFACE_WINDOW_EXCEPTIONS_H_
