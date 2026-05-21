// Copyright 2026 maxim (necromax) alekseenko

#ifndef TOOLS_LUMCOMPILER_EXCEPTIONS_H_
#define TOOLS_LUMCOMPILER_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace lumcompiler {

struct LumcompilerError : public std::runtime_error {
  explicit LumcompilerError(const std::string& message)
      : std::runtime_error(message) {}
};

struct MissingLumdataError : public LumcompilerError {
  explicit MissingLumdataError(const std::string& message)
      : LumcompilerError(message) {}
};

struct InvalidLumdataError : public LumcompilerError {
  explicit InvalidLumdataError(const std::string& message)
      : LumcompilerError(message) {}
};

}  // namespace lumcompiler

#endif  // TOOLS_LUMCOMPILER_EXCEPTIONS_H_
