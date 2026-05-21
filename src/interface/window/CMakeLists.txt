add_library(interfaceengine_window STATIC)
add_library(interfaceengine::window ALIAS interfaceengine_window)

target_link_libraries(interfaceengine_window
    PUBLIC 
        interfaceengine_interface
        SDL3::SDL3
        necroutils::necroutils#TODO remove
)

target_sources(interfaceengine_window
    PRIVATE
        manager.cc
)