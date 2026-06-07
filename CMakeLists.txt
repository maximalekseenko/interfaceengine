cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(interfaceengine
    VERSION 1.0.0
    DESCRIPTION "Necromax's engine for 2d applications."
    LANGUAGES CXX
)

# -------------------- OPTIONS --------------------
option(INTERFACEENGINE_INSTALL "Install interfaceengine and it's dependencies" ${PROJECT_IS_TOP_LEVEL})
option(INTERFACEENGINE_INSTALL_DEFAULT_LUMC "Install default lumcompiler after compiling" ${PROJECT_IS_TOP_LEVEL})

# -------------------- FETCH LIBRARIES --------------------
include(FetchContent)

# Fetch SDL3.
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        release-3.2.30
    FIND_PACKAGE_ARGS 3.2
)
FetchContent_MakeAvailable(SDL3)

# Fetch SDL3_image.
set(SDLIMAGE_VENDORED ON)
FetchContent_Declare(SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG        release-3.2.6
    FIND_PACKAGE_ARGS 3.2
)
FetchContent_MakeAvailable(SDL3_image)

# Fetch SDL3_ttf.
set(SDLTTF_VENDORED ON)
set(SDLTTF_HARFBUZZ OFF) # disabled for now
FetchContent_Declare(SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG        release-3.2.2
    FIND_PACKAGE_ARGS 3.2
)
FetchContent_MakeAvailable(SDL3_ttf)

# Fetch necroutils.
FetchContent_Declare(necroutils
    GIT_REPOSITORY https://github.com/maximalekseenko/necroutils
    GIT_TAG        60c5d393018a308a32620def47fe89dfddaf0e1f
    FIND_PACKAGE_ARGS 1.1
)
FetchContent_MakeAvailable(necroutils)

# -------------------- TARGET DECLARATION INCLUDES --------------------
include(GNUInstallDirs)

# -------------------- SETUP INTERFACE --------------------
add_library(interfaceengine_interface INTERFACE)

target_compile_features(interfaceengine_interface  INTERFACE cxx_std_20)

set_target_properties(interfaceengine_interface PROPERTIES CXX_STANDARD_REQUIRED ON)

target_compile_definitions(interfaceengine_interface INTERFACE INTERFACE_LOCAL)

if(MSVC)
    target_compile_options(interfaceengine_interface INTERFACE
        /W4
        /permissive-
    )
else()
    target_compile_options(interfaceengine_interface INTERFACE
        -W
        -Wall 
        -Wcast-qual
        -Wextra
        -Wsign-compare
        -Wundef
        -Wpointer-arith
        -Wwrite-strings
        -Wredundant-decls
        -Wformat-security
        -Wformat=2
        -Winit-self
        -Wnon-virtual-dtor
        -Wsuggest-override
    )
endif()

if(MINGW)
    target_link_options(interfaceengine_interface INTERFACE -static -static-libgcc -static-libstdc++)
endif()

target_include_directories(interfaceengine_interface 
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# -------------------- COMPILE SUBMODULES --------------------
add_subdirectory("src/interface/lumen")

add_subdirectory("src/interface/window")

add_subdirectory("src/interface/gui")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tools/lumcompiler")

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(interfaceengine
    PRIVATE
        interfaceengine_interface
        interfaceengine_lumen
        interfaceengine_window
        interfaceengine_gui
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
    PUBLIC
        $<BUILD_INTERFACE:necroutils::necroutils>
)

target_sources(interfaceengine
    PRIVATE
        src/interface/misc/rect.cpp
        src/interface/core/interface.cpp
        src/interface/core/interface_impl.cpp
)

# -------------------- LINT CHECK --------------------
include(necroutilsRunCpplint)
necroutils_RunCpplint(interfaceengine 
    DIRECTORIES 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# -------------------- INSTALLATION --------------------
include(CMakePackageConfigHelpers)

if(INTERFACEENGINE_INSTALL)
    install(TARGETS interfaceengine
        EXPORT interfaceengineTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY include/ 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT interfaceengineTargets
        FILE interfaceengineTargets.cmake
        NAMESPACE interfaceengine::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/interfaceengine
    )

    set(targets_export_name interfaceengineTargets)

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/interfaceengine-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/interfaceengineConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/interfaceengine
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/interfaceengineConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/interfaceengineConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/interfaceengineConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/interfaceengine
    )
endif()