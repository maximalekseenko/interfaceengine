/// @file dll.h
/// @author maxim (necromax) alekseenko
/// @brief Defines `DLL_PUBLIC` and `DLL_LOCAL` 
/// used for this library compilation.

#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(BUILDING_DLL)
    #if defined(__GNUC__)
      #define DLL_PUBLIC __attribute__((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #if defined(__GNUC__)
      #define DLL_PUBLIC __attribute__((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__((visibility("default")))
    #define DLL_LOCAL  __attribute__((visibility("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif