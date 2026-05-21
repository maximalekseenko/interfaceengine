add_executable(lumcompiler)


set_target_properties(lumcompiler PROPERTIES 
        OUTPUT_NAME "lumc"
        PREFIX ""
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(lumcompiler
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/tools
)

target_link_libraries(lumcompiler  
    PRIVATE 
        interfaceengine_interface
        necroutils::necroutils
        interfaceengine::lumen::package
        SDL3::SDL3
)

target_sources(lumcompiler
    PRIVATE
        main.cc
        cpackage.cc
)