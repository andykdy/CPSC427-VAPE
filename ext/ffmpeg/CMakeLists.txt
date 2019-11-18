if (IS_OS_LINUX OR IS_OS_MAC)
    find_package(PkgConfig REQUIRED)

    pkg_search_module(LIBAVCODEC REQUIRED libavcodec)
    pkg_search_module(LIBAVFORMAT REQUIRED libavformat)
    pkg_search_module(LIBAVFILTER REQUIRED libavfilter)
    pkg_search_module(LIBAVDEVICE REQUIRED libavdevice)
    pkg_search_module(LIBAVUTIL REQUIRED libavutil)
    pkg_search_module(LIBSWRESAMPLE REQUIRED libswresample)
    pkg_search_module(LIBSWSCALE REQUIRED libswscale)
elseif (IS_OS_WINDOWS)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
        set(LIBAVCODEC_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(LIBAVFORMAT_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(LIBAVFILTER_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(LIBAVDEVICE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(LIBAVUTIL_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(LIBSWRESAMPLE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")
        set(LIBSWSCALE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win64/include/")

        set(LIBAVCODEC_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avcodec.lib")
        set(LIBAVFORMAT_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avformat.lib")
        set(LIBAVFILTER_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avfilter.lib")
        set(LIBAVDEVICE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avdevice.lib")
        set(LIBAVUTIL_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/avutil.lib")
        set(LIBSWRESAMPLE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/swresample.lib")
        set(LIBSWSCALE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win64/lib/swscale.lib")

        set(LIBAVCODEC_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avcodec-58.dll" PARENT_SCOPE)
        set(LIBAVFORMAT_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avformat-58.dll" PARENT_SCOPE)
        set(LIBAVFILTER_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avfilter-7.dll" PARENT_SCOPE)
        set(LIBAVDEVICE_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avdevice-58.dll" PARENT_SCOPE)
        set(LIBAVUTIL_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/avutil-56.dll" PARENT_SCOPE)
        set(LIBSWRESAMPLE_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/swresample-3.dll" PARENT_SCOPE)
        set(LIBSWSCALE_DLL "${CMAKE_CURRENT_LIST_DIR}/win64/bin/swscale-5.dll" PARENT_SCOPE)
    else()
        set(LIBAVCODEC_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(LIBAVFORMAT_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(LIBAVFILTER_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(LIBAVDEVICE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(LIBAVUTIL_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(LIBSWRESAMPLE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")
        set(LIBSWSCALE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/win32/include/")

        set(LIBAVCODEC_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avcodec.lib")
        set(LIBAVFORMAT_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avformat.lib")
        set(LIBAVFILTER_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avfilter.lib")
        set(LIBAVDEVICE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avdevice.lib")
        set(LIBAVUTIL_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/avutil.lib")
        set(LIBSWRESAMPLE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/swresample.lib")
        set(LIBSWSCALE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/win32/lib/swscale.lib")

        set(LIBAVCODEC_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avcodec-58.dll" PARENT_SCOPE)
        set(LIBAVFORMAT_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avformat-58.dll" PARENT_SCOPE)
        set(LIBAVFILTER_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avfilter-7.dll" PARENT_SCOPE)
        set(LIBAVDEVICE_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avdevice-58.dll" PARENT_SCOPE)
        set(LIBAVUTIL_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/avutil-56.dll" PARENT_SCOPE)
        set(LIBSWRESAMPLE_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/swresample-3.dll" PARENT_SCOPE)
        set(LIBSWSCALE_DLL "${CMAKE_CURRENT_LIST_DIR}/win32/bin/swscale-5.dll" PARENT_SCOPE)
    endif()
endif()

set(FFMPEG_INCLUDE_DIRS
        ${LIBAVCODEC_INCLUDE_DIRS}
        ${LIBAVFORMAT_INCLUDE_DIRS}
        ${LIBAVFILTER_INCLUDE_DIRS}
        ${LIBAVDEVICE_INCLUDE_DIRS}
        ${LIBAVUTIL_INCLUDE_DIRS}
        ${LIBSWRESAMPLE_INCLUDE_DIRS}
        ${LIBSWSCALE_INCLUDE_DIRS}
        PARENT_SCOPE)
set(FFMPEG_LIBRARIES
        ${LIBAVCODEC_LIBRARIES}
        ${LIBAVFORMAT_LIBRARIES}
        ${LIBAVFILTER_LIBRARIES}
        ${LIBAVDEVICE_LIBRARIES}
        ${LIBAVUTIL_LIBRARIES}
        ${LIBSWRESAMPLE_LIBRARIES}
        ${LIBSWSCALE_LIBRARIES}
        PARENT_SCOPE)