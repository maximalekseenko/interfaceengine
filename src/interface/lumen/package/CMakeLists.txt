add_library(interfaceengine_lumen_package STATIC)
add_library(interfaceengine::lumen::package ALIAS interfaceengine_lumen_package)

target_link_libraries(interfaceengine_lumen_package 
    PRIVATE
        interfaceengine_interface
        necroutils::necroutils
    PUBLIC
        interfaceengine_lumen_object
)

target_sources(interfaceengine_lumen_package
    PRIVATE
        package.cc
        loader.cc
)