add_executable(lumc)
add_executable(interfaceengine::lumc ALIAS lumc)

set_target_properties(lumc PROPERTIES 
        OUTPUT_NAME "lumc"
        PREFIX ""
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(lumc
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/tools
)

target_link_libraries(lumc  
    PRIVATE 
        interfaceengine_interface
        interfaceengine::lumen::package
        SDL3::SDL3
)

target_sources(lumc
    PRIVATE
        main.cc
)

if(INTERFACEENGINE_INSTALL_DEFAULT_LUMC)
    install(TARGETS lumc
        EXPORT interfaceengineTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()