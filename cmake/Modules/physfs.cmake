
# Download and unpack physfs at configure time
macro(fetch_physfs _download_module_path _download_root)


    set(PHYSFS_DOWNLOAD_ROOT ${_download_root})
    configure_file(
            ${_download_module_path}/physfs-download.cmake
            ${_download_root}/CMakeLists.txt
            @ONLY
    )
    unset(PHYSFS_DOWNLOAD_ROOT)

    execute_process(
            COMMAND
            ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
            RESULT_VARIABLE
            result
            WORKING_DIRECTORY
            ${_download_root}
    )
    if(result)
        message(FATAL_ERROR CMake step for physfs failed: ${result})
    endif()

    execute_process(
            COMMAND
            ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE
            result
            WORKING_DIRECTORY
            ${_download_root}
    )
    if(result)
        message(FATAL_ERROR Build step for physfs failed: ${result})
    endif()

    # Add physfs directly to our build.
    add_subdirectory(
            ${_download_root}/physfs-src
            ${_download_root}/physfs-build
            EXCLUDE_FROM_ALL
    )

    target_include_directories(${PROJECT_NAME} PUBLIC  ${_download_root}/physfs-src/src)

    if (IS_OS_WINDOWS)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_download_root}/physfs-build/libphysfs.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libphysfs.dll")
    endif()

endmacro()