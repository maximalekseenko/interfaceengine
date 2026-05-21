cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

# -------------------- PROJECT --------------------
project(interfaceengine
    VERSION 1.0.0
    DESCRIPTION "Necromax's engine for 2d applications."
    LANGUAGES CXX
)

# -------------------- OPTIONS --------------------
option(INTERFACEENGINE_RUN_CPPLINT "Run cpplint (Google style tests) when building" ON)
option(INTERFACEENGINE_COMPILE_LUMCOMPILER "Compile lumcompiler additional tool" ON)

# -------------------- FETCH LIBRARIES --------------------
include(FetchContent)

# Fetch SDL3.
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        3.4.8
    FIND_PACKAGE_ARGS REQUIRED)
FetchContent_MakeAvailable(SDL3)

# Fetch SDL3_image.
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG        3.4.4
    FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(SDL3_image)

# Fetch SDL3_ttf.
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG        3.1.0
    FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(SDL3_ttf)

# Fetch necroutils.
FetchContent_Declare(
    necroutils
    GIT_REPOSITORY https://github.com/maximalekseenko/necroutils
    GIT_TAG        78208b2aebb8dbff1bfc8f1b1173882e883ccd04
    FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(necroutils)


# # -------------------- SETUP INTERFACE --------------------
add_library(interfaceengine_interface INTERFACE)

target_compile_features(interfaceengine_interface  INTERFACE cxx_std_20)

if(MSVC)
    target_compile_options(interfaceengine_interface INTERFACE /W4 /permissive-)
else()
    target_compile_options(interfaceengine_interface INTERFACE -Wall -Wpedantic -Wshadow -Wnon-virtual-dtor)
endif()

target_compile_definitions(interfaceengine_interface INTERFACE BUILDING_DLL) # TODO(necromax) rename

target_include_directories(interfaceengine_interface 
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# -------------------- COMPILE SUBMODULES --------------------
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/interface/lumen")

if(INTERFACEENGINE_COMPILE_LUMCOMPILER)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tools/lumcompiler")
    add_dependencies(interfaceengine_interface interfaceengine_lumcompiler)
endif()

# -------------------- LIBRARY TARGET --------------------
add_library(interfaceengine SHARED)
add_library(interfaceengine::interfaceengine ALIAS interfaceengine)

set_target_properties(interfaceengine PROPERTIES
    OUTPUT_NAME "interface"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(interfaceengine
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(interfaceengine
    PRIVATE
        interfaceengine_interface
        necroutils::necroutils
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        interfaceengine::lumen::manager
)

target_sources(interfaceengine
    PRIVATE
        src/interface/misc/rect.cpp
        src/interface/gui/gui_component.cpp
        src/interface/core/interface.cpp
        src/interface/window_manager/window_manager.cpp
)


# -------------------- CPPLINT --------------------
if(INTERFACEENGINE_RUN_CPPLINT)
    include(FetchContent)
    FetchContent_Declare(
        cpplint
        GIT_REPOSITORY https://github.com/cpplint/cpplint/
        GIT_TAG        2.0.2)
    FetchContent_MakeAvailable(cpplint)

    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    add_custom_command(
        TARGET interfaceengine POST_BUILD
        COMMAND ${Python3_EXECUTABLE}
        ${cpplint_SOURCE_DIR}/cpplint.py
        "--recursive"
        "--quiet"
        "--root" "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/tools"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/interface/lumen/object"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/interface/lumen/package"
        # "${CMAKE_CURRENT_SOURCE_DIR}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running cpplint (Google styles tests)"
    )
endif()