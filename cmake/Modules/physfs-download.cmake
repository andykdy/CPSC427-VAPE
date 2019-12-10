cmake_minimum_required(VERSION 3.10)

project(physfs-download NONE)

include(ExternalProject)

ExternalProject_Add(
        physfs
        URL               https://icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2
        SOURCE_DIR        "@PHYSFS_DOWNLOAD_ROOT@/physfs-src"
        BINARY_DIR        "@PHYSFS_DOWNLOAD_ROOT@/physfs-build"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
)