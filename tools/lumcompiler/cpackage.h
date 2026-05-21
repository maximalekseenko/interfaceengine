// Copyright 2026 maxim (necromax) alekseenko

#ifndef TOOLS_LUMCOMPILER_CPACKAGE_H_
#define TOOLS_LUMCOMPILER_CPACKAGE_H_

#include <SDL3/SDL_render.h>

#include <string>

#include "interface/dll.h"
#include "interface/lumen/package/package.h"

namespace lumcompiler {

class DLL_LOCAL LumCompilingPackage
    : public interfaceengine::lumen::package::LumPackage {
 public:
  struct CompileParameters {
    //
  };

 public:
  LumCompilingPackage() = default;

 public:
  void CompileLumFile(const std::string& lumdata_path, std::string lum_out_path,
                      SDL_Renderer* renderer);
};

}  // namespace lumcompiler

#endif  // TOOLS_LUMCOMPILER_CPACKAGE_H_
