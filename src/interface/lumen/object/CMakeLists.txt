add_library(interfaceengine_lumen_object STATIC)
add_library(interfaceengine::lumen::object ALIAS interfaceengine_lumen_object)

target_link_libraries(interfaceengine_lumen_object
    PRIVATE
        SDL3_image::SDL3_image
        interfaceengine_interface
    PUBLIC
        SDL3::SDL3
)

target_sources(interfaceengine_lumen_object
    PRIVATE
        asset_image.cc
        object.cc
        sdl_io.cc
        lumen_image.cc
        sdl_extract.cc
)