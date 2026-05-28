add_library(interfaceengine_gui STATIC)
add_library(interfaceengine::gui ALIAS interfaceengine_gui)

target_link_libraries(interfaceengine_gui
    PRIVATE
        necroutils::necroutils
    PUBLIC 
        interfaceengine_interface
        SDL3::SDL3
)

target_sources(interfaceengine_gui
    PRIVATE
        component.cc
        manager.cc
)