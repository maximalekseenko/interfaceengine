add_library(interfaceengine_lumen_manager STATIC)
add_library(interfaceengine::lumen::manager ALIAS interfaceengine_lumen_manager)

target_link_libraries(interfaceengine_lumen_manager
    PUBLIC 
        interfaceengine_interface
        interfaceengine::lumen::package
        SDL3::SDL3
)

target_sources(interfaceengine_lumen_manager
    PRIVATE
        manager.cc
)