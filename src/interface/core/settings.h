/// @file settings.h
/// @author maxim (necromax) alekseenko
/// @brief Defines settings for interface.

#pragma once

#include <filesystem>

#include "interface/dll.h"

/// @brief A structure that contains settings for interface instance.
class DLL_LOCAL InterfaceSettings {
   public:
    /// @brief A path to the executable file.
    std::filesystem::path executable_path{""};

    /// @brief A path folder with assets
    /// relative to executable.
    std::filesystem::path asset_folder{"./"};

    /// @brief A maximum amount of frames per second permited.
    unsigned int fps = 30;
};