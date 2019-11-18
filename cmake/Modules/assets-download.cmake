cmake_minimum_required(VERSION 3.10)

function(download_assets)
        # TODO some way of checking if file is up to date?
        # TODO move onto my home server for faster speeds?
        if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data/data.7z)
                file(DOWNLOAD https://cdn-16.anonfile.com/Y8f2deB3n1/a07f9851-1574074243/data.7z ${CMAKE_CURRENT_SOURCE_DIR}/data/data.7z SHOW_PROGRESS)
                execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xfvz ${CMAKE_CURRENT_SOURCE_DIR}/data/data.7z
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)
        endif()
endfunction(download_assets)

#[[
include(ExternalProject)

ExternalProject_Add(
        assets
        URL http://download.osgvisual.org/3rdParty_VS2013_v120_x86_x64_V9_small.7z # TODO change
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ./ ${CMAKE_CURRENT_SOURCE_DIR}/data
        )
)]]


#[[

include(FetchContent)
FetchContent_Populate(
        assets
        URL        http://download.osgvisual.org/3rdParty_VS2013_v120_x86_x64_V9_small.7z
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data
)]]
