add_library(interfaceengine_lumen STATIC)
add_library(interfaceengine::lumen::manager ALIAS interfaceengine_lumen)

target_link_libraries(interfaceengine_lumen
    PUBLIC 
        interfaceengine_interface
        interfaceengine_lumen_package
        SDL3::SDL3
)

target_sources(interfaceengine_lumen
    PRIVATE
        manager.cc
)

add_subdirectory("object")
add_subdirectory("package")