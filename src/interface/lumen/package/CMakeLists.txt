add_library(interfaceengine_lumen_package STATIC)
add_library(interfaceengine::lumen::package ALIAS interfaceengine_lumen_package)

target_link_libraries(interfaceengine_lumen_package 
    PRIVATE
        necroutils::necroutils
    PUBLIC
        interfaceengine_interface
        interfaceengine_lumen_object
)

target_sources(interfaceengine_lumen_package
    PRIVATE
        package.cc
)