// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_OBJECT_SDL_IO_H_
#define SRC_INTERFACE_LUMEN_OBJECT_SDL_IO_H_

#include <SDL3/SDL_iostream.h>

#include <istream>

#include "interface/dll.h"

namespace interfaceengine::lumen::object {

DLL_LOCAL SDL_IOStream* OpenIOFromIStream(std::istream& cppStream);

}  // namespace interfaceengine::lumen::object

#endif  // SRC_INTERFACE_LUMEN_OBJECT_SDL_IO_H_
