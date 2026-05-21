// Copyright 2026 maxim (necromax) alekseenko

#include <SDL3/SDL.h>

#include <string>

#include "lumcompiler/cpackage.h"
#include "lumcompiler/exceptions.h"

int main(int argc, char* argv[]) {
  if (argc < 2)  // TODO(necromax): make a better way for getting parameters.
    throw lumcompiler::LumcompilerError("No path to lumdata file provided.");
  if (argc < 3)
    throw lumcompiler::LumcompilerError(
        "No path to output directory provided.");
  if (argc > 3)  // TODO(necromax): process additional parameters.
    throw lumcompiler::LumcompilerError("Too many arguments provided.");

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

  lumcompiler::LumCompilingPackage package;
  package.CompileLumFile(argv[1], argv[2], renderer);
}
