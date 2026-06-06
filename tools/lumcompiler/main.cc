// Copyright 2026 maxim (necromax) alekseenko

#include <SDL3/SDL.h>

#include <iostream>
#include <string>

#include "interface/lumen/package/loader.h"
#include "lumcompiler/exceptions.h"

void PrintHelp() {
  std::cout << R"(Lum Package Compiler -- a tool for compiling .lum files.
Usage:
  lumc <input-path> <output-path> [options]
Arguments:
  <input-path>    Path to *.lumdata file.
  <output-path>   Path to file that will be written.
Options:
  -d, --dir.      If set, <output-path> is treated as an output directory insted.
                  Name of the output file will be same as input *.lumdata file.
)";
}

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 3) {
    PrintHelp();
    return 1;
  }

  SDL_Surface* surface = SDL_CreateSurface(800, 600, SDL_PIXELFORMAT_RGBA8888);
  if (!surface) {
    SDL_Log("Could not create surface: %s", SDL_GetError());
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
  if (!renderer) {
    SDL_Log("Could not create renderer: %s", SDL_GetError());
    SDL_DestroySurface(surface);
    return 1;
  }

  interfaceengine::lumen::package::LumPackageLoader package_loader;

  package_loader.CompileFromLumdataFile(argv[1], argv[2], renderer);

  return 0;
}
