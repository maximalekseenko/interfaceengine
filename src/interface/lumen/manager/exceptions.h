// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_MANAGER_EXCEPTIONS_H_
#define SRC_INTERFACE_LUMEN_MANAGER_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

#include "interface/dll.h"

namespace lumen::manager {

class DLL_LOCAL ManagerException : public std::runtime_error {
 public:
  explicit ManagerException(const std::string& message)
      : std::runtime_error(message) {}
};

class DLL_LOCAL PackageNotFoundError : public ManagerException {
 public:
  explicit PackageNotFoundError(const std::string& function_name,
                                const std::string& package_name)
      : ManagerException(function_name + ": Package \"" + package_name
                         + "\" does not exist.") {}
};

class DLL_LOCAL PackageAlreadyExistsError : public ManagerException {
 public:
  explicit PackageAlreadyExistsError(const std::string& function_name,
                                     const std::string& package_name)
      : ManagerException(function_name + ": Package \"" + package_name
                         + "\" already exists.") {}
};

}  // namespace lumen::manager

#endif  // SRC_INTERFACE_LUMEN_MANAGER_EXCEPTIONS_H_
