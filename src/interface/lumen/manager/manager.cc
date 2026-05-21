// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/manager/manager.h"

#include <SDL3/SDL_render.h>

#include <cstdint>
#include <fstream>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>

#include "interface/lumen/manager/exceptions.h"
#include "interface/lumen/package/package.h"

namespace lumen::manager {

void LumenManager::LoadLumPackage(const std::string& path,
                                  SDL_Renderer* renderer) {
  auto new_package = std::make_unique<package::LumPackage>();

  new_package->LoadPackage(path, renderer);

  auto [it, inserted] = loader_packages_.try_emplace(new_package->name(),
                                                     std::move(new_package));
  // TODO(necromax): throw if exists.
}

SDL_Texture* LumenManager::GetLumenTexture(const std::string& package_name,
                                           const std::string& lumen_name,
                                           std::string lumen_data) {
  auto it = loader_packages_.find(package_name);
  if (it == loader_packages_.end())
    throw PackageNotFoundError("LumenManager::GetLumenTexture", package_name);

  return it->second->GetLumenTexture(lumen_name);
}

void LumenManager::RemovePackage(const std::string& package_name) {
  if (loader_packages_.erase(package_name) == 0)
    throw PackageNotFoundError("LumenManager::RemovePackage", package_name);
}

}  // namespace lumen::manager