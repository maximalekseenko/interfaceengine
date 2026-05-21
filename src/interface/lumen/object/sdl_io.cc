// Copyright 2026 maxim (necromax) alekseenko

#include "interface/lumen/object/sdl_io.h"

#include <SDL3/SDL_iostream.h>

#include <istream>

namespace interfaceengine::lumen::object {

static size_t SDLCALL StreamRead(void* userdata, void* ptr, size_t size,
                                 SDL_IOStatus* status) {
  auto* stream = static_cast<std::istream*>(userdata);
  stream->read(static_cast<char*>(ptr), static_cast<std::streamsize>(size));

  size_t bytesRead = static_cast<size_t>(stream->gcount());

  if (bytesRead == 0 && stream->eof()) {
    if (status) *status = SDL_IO_STATUS_EOF;
  }

  return bytesRead;
}

static Sint64 SDLCALL StreamSeek(void* userdata, Sint64 offset,
                                 SDL_IOWhence whence) {
  auto* stream = static_cast<std::istream*>(userdata);

  std::ios_base::seekdir dir;
  switch (whence) {
    case SDL_IO_SEEK_SET:
      dir = std::ios_base::beg;
      break;
    case SDL_IO_SEEK_CUR:
      dir = std::ios_base::cur;
      break;
    case SDL_IO_SEEK_END:
      dir = std::ios_base::end;
      break;
    default:
      return -1;
  }

  stream->clear();  // important: clear EOF/fail flags before seeking
  stream->seekg(static_cast<std::streamoff>(offset), dir);

  if (stream->fail()) {
    return -1;
  }

  auto pos = stream->tellg();
  if (pos == std::istream::pos_type(-1)) {
    return -1;
  }

  return static_cast<Sint64>(pos);
}

SDL_IOStream* OpenIOFromIStream(std::istream& cppStream) {
  SDL_IOStreamInterface iface;
  SDL_INIT_INTERFACE(&iface);

  iface.read = StreamRead;
  iface.seek = StreamSeek;

  return SDL_OpenIO(&iface, &cppStream);
}

}  // namespace interfaceengine::lumen::object
