cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

ExternalProject_Add(
        assets
        URL http://download.osgvisual.org/3rdParty_VS2013_v120_x86_x64_V9_small.7z # TODO change
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ./ ${CMAKE_CURRENT_SOURCE_DIR}/data
        )



#[[

include(FetchContent)
FetchContent_Populate(
        assets
        URL        http://download.osgvisual.org/3rdParty_VS2013_v120_x86_x64_V9_small.7z
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data
)]]
