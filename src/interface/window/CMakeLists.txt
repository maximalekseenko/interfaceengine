add_library(interfaceengine_window STATIC)
add_library(interfaceengine::window ALIAS interfaceengine_window)

target_link_libraries(interfaceengine_window
    PRIVATE
        interfaceengine_interface
    PUBLIC 
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
)

target_sources(interfaceengine_window
    PRIVATE
        manager.cc
)