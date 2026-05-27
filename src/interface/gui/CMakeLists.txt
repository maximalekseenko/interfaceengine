add_library(interfaceengine_lumen_gui STATIC)
add_library(interfaceengine::lumen::gui ALIAS interfaceengine_lumen_gui)

target_link_libraries(interfaceengine_lumen_gui
    PUBLIC 
        interfaceengine_interface
        SDL3::SDL3
)

target_sources(interfaceengine_lumen_gui
    PRIVATE
        # gui_component.cc
        manager.cc
)