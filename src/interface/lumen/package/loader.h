// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_PACKAGE_LOADER_H_
#define SRC_INTERFACE_LUMEN_PACKAGE_LOADER_H_

#include <string>
#include <unordered_map>

#include "interface/dll.h"
#include "interface/lumen/package/package.h"

namespace interfaceengine::lumen::package {

class INTERFACE_INTERNAL LumPackageLoader {
 public:
  LumPackageLoader() = default;
  ~LumPackageLoader() = default;

 public:
  LumPackage::Ptr Load(const std::string& package_path, SDL_Renderer* renderer);
  LumPackage::Ptr LoadFromLumFile(const std::string& package_path,
                                  SDL_Renderer* renderer);
  LumPackage::Ptr LoadFromLumdataFile(const std::string& package_path,
                                      SDL_Renderer* renderer);
  LumPackage::Ptr LoadFromFolder(const std::string& package_path,
                                 SDL_Renderer* renderer);

  void CompileFromLumdataFile(const std::string& package_path,
                              std::string out_path, SDL_Renderer* renderer);
};

}  // namespace interfaceengine::lumen::package

#endif  // SRC_INTERFACE_LUMEN_PACKAGE_LOADER_H_
