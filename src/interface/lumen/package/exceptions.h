// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_LUMEN_PACKAGE_EXCEPTIONS_H_
#define SRC_INTERFACE_LUMEN_PACKAGE_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

#include "interface/dll.h"

namespace interfaceengine::lumen::package {

class DLL_LOCAL LumPackageException : public std::runtime_error {
 public:
  explicit LumPackageException(const std::string& message)
      : std::runtime_error(message) {}
};

#define DEFINE_LUM_PACKAGE_EXCEPTION(name)            \
  class DLL_LOCAL name : public LumPackageException { \
   public:                                            \
    explicit name(const std::string& message)         \
        : LumPackageException(message) {}             \
  };

DEFINE_LUM_PACKAGE_EXCEPTION(InvalidMetaStructure)
DEFINE_LUM_PACKAGE_EXCEPTION(MetaEmtpyLine)
DEFINE_LUM_PACKAGE_EXCEPTION(MetaCommentedLine)
DEFINE_LUM_PACKAGE_EXCEPTION(LumdataNotFound)
DEFINE_LUM_PACKAGE_EXCEPTION(InvalidLumdataError)
DEFINE_LUM_PACKAGE_EXCEPTION(DuplicateObject)
DEFINE_LUM_PACKAGE_EXCEPTION(ObjectNotFound)

#undef DEFINE_LUM_PACKAGE_EXCEPTION

}  // namespace interfaceengine::lumen::package

#endif  // SRC_INTERFACE_LUMEN_PACKAGE_EXCEPTIONS_H_
