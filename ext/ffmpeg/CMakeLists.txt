if (IS_OS_LINUX OR IS_OS_MAC)
    pkg_check_modules(AVCODEC REQUIRED)
    pkg_check_modules(AVFORMAT REQUIRED)
    pkg_check_modules(AVFILTER REQUIRED)
    pkg_check_modules(AVDEVICE REQUIRED)
    pkg_check_modules(AVUTIL REQUIRED)
    pkg_check_modules(SWRESAMPLE REQUIRED)
    pkg_check_modules(SWSCALE REQUIRED)
elseif (IS_OS_WINDOWS)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
        set(AVCODEC_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(AVFORMAT_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(AVFILTER_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(AVDEVICE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(AVUTIL_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(SWRESAMPLE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(SWSCALE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")

        set(AVCODEC_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avcodec.lib")
        set(AVFORMAT_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avformat.lib")
        set(AVFILTER_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avfilter.lib")
        set(AVDEVICE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avdevice.lib")
        set(AVUTIL_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avutil.lib")
        set(SWRESAMPLE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/swresample.lib")
        set(SWSCALE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/swscale.lib")

        set(AVCODEC_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avcodec-58.dll" PARENT_SCOPE)
        set(AVFORMAT_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avformat-58.dll" PARENT_SCOPE)
        set(AVFILTER_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avfilter-7.dll" PARENT_SCOPE)
        set(AVDEVICE_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avdevice-58.dll" PARENT_SCOPE)
        set(AVUTIL_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avutil-56.dll" PARENT_SCOPE)
        set(SWRESAMPLE_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/swresample-3.dll" PARENT_SCOPE)
        set(SWSCALE_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/swscale-5.dll" PARENT_SCOPE)
    else()
        set(AVCODEC_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(AVFORMAT_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(AVFILTER_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(AVDEVICE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(AVUTIL_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(SWRESAMPLE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(SWSCALE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")

        set(AVCODEC_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avcodec.lib")
        set(AVFORMAT_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avformat.lib")
        set(AVFILTER_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avfilter.lib")
        set(AVDEVICE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avdevice.lib")
        set(AVUTIL_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avutil.lib")
        set(SWRESAMPLE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/swresample.lib")
        set(SWSCALE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/swscale.lib")

        set(AVCODEC_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avcodec-58.dll" PARENT_SCOPE)
        set(AVFORMAT_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avformat-58.dll" PARENT_SCOPE)
        set(AVFILTER_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avfilter-7.dll" PARENT_SCOPE)
        set(AVDEVICE_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avdevice-58.dll" PARENT_SCOPE)
        set(AVUTIL_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avutil-56.dll" PARENT_SCOPE)
        set(SWRESAMPLE_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/swresample-3.dll" PARENT_SCOPE)
        set(SWSCALE_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/swscale-5.dll" PARENT_SCOPE)
    endif()
endif()

set(FFMPEG_INCLUDE_DIRS
        ${AVCODEC_INCLUDE_DIRS}
        ${AVFORMAT_INCLUDE_DIRS}
        ${AVFILTER_INCLUDE_DIRS}
        ${AVDEVICE_INCLUDE_DIRS}
        ${AVUTIL_INCLUDE_DIRS}
        ${SWRESAMPLE_INCLUDE_DIRS}
        ${SWSCALE_INCLUDE_DIRS}
        PARENT_SCOPE)
set(FFMPEG_LIBRARIES
        ${AVCODEC_LIBRARIES}
        ${AVFORMAT_LIBRARIES}
        ${AVFILTER_LIBRARIES}
        ${AVDEVICE_LIBRARIES}
        ${AVUTIL_LIBRARIES}
        ${SWRESAMPLE_LIBRARIES}
        ${SWSCALE_LIBRARIES}
        PARENT_SCOPE)