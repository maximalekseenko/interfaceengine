// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_CORE_SETTINGS_H_
#define SRC_INTERFACE_CORE_SETTINGS_H_

#include <string>

#include "interface/dll.h"

/// @brief A structure that contains settings for interface instance.
class DLL_LOCAL InterfaceSettings {
 public:
  /// @brief A path to the executable file.
  std::string executable_path{""};

  /// @brief A path folder with assets
  /// relative to executable.
  std::string asset_folder{"./"};

  /// @brief A maximum amount of frames per second permited.
  unsigned int fps = 30;
};

#endif  // SRC_INTERFACE_CORE_SETTINGS_H_
