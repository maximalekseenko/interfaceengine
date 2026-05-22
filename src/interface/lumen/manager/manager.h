// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_MANAGER_MANAGER_H_
#define SRC_INTERFACE_LUMEN_MANAGER_MANAGER_H_

#include <SDL3/SDL_render.h>

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>

#include "interface/dll.h"
#include "interface/lumen/package/package.h"

namespace interfaceengine::lumen::manager {

/// @brief A class for loading and managing lumens.
class INTERFACE_INTERNAL LumenManager {
 public:  // -------------------- TYPES --------------------
  using LumPackagePtr = std::unique_ptr<package::LumPackage>;

 public:  // -------------------- CONSTRUCTORS --------------------
  LumenManager() = default;

 public:  // -------------------- DESTRUCTORS --------------------
  ~LumenManager() = default;

 public:  // -------------------- PUBLIC METHODS --------------------
  /// @brief Loads a lum package of any type.
  /// @param path A path to a lumen package to load.
  /// @param renderer Renderer that will be used to render lumens.
  void LoadLumPackage(const std::string& path, SDL_Renderer* renderer);

  /// @brief Retrieves SDL_Texture from a lumen.
  /// @param package_name A name of a package.
  /// that had contained the lumen to get the texture from.
  /// @param lumen_name A name of a lumen to get the texture from.
  /// @return A texture for further rendering.
  SDL_Texture* GetLumenTexture(const std::string& package_name,
                               const std::string& lumen_name,
                               std::string lumen_data);

  /// @brief Removes a package from registry.
  /// @param package_name A name of a package
  /// that had contained the lumens to remove.
  void RemovePackage(const std::string& package_name);

 private:  // -------------------- PRIVATE MEMBERS --------------------
  /// @brief A registry of loaded packages.
  std::unordered_map<std::string, LumPackagePtr> loader_packages_;
};

}  // namespace interfaceengine::lumen::manager

#endif  // SRC_INTERFACE_LUMEN_MANAGER_MANAGER_H_
