// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_DLL_H_
#define INCLUDE_INTERFACE_DLL_H_

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(INTERFACE_LOCAL)
    #if defined(__GNUC__)
      #define INTERFACE_API __attribute__((dllexport))
    #else
      #define INTERFACE_API __declspec(dllexport)
    #endif
  #else
    #if defined(__GNUC__)
      #define INTERFACE_API __attribute__((dllimport))
    #else
      #define INTERFACE_API __declspec(dllimport)
    #endif
  #endif
  #define INTERFACE_INTERNAL
#else
  #if __GNUC__ >= 4
    #define INTERFACE_API __attribute__((visibility("default")))
    #define INTERFACE_INTERNAL __attribute__((visibility("hidden")))
  #else
    #define INTERFACE_API
    #define INTERFACE_INTERNAL
  #endif
#endif

#endif  // INCLUDE_INTERFACE_DLL_H_
