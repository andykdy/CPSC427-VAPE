cmake_minimum_required(VERSION 3.10)

function(download_assets)
        # TODO some way of checking if file is up to date?
        # TODO move onto my home server for faster speeds?
        if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data.7z)
                message("Downloading video assets...")
                file(DOWNLOAD https://dl.dropboxusercontent.com/s/2c2xrvs1uf3admm/data.7z?dl=0 ${CMAKE_CURRENT_SOURCE_DIR}/data/data.7z SHOW_PROGRESS)
                message("Downloaded")
                execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xfvz ${CMAKE_CURRENT_SOURCE_DIR}/data.7z
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)
        endif()
endfunction(download_assets)

