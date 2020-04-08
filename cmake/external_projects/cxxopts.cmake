cmake_minimum_required(VERSION 3.10.2)
cmake_policy(VERSION 3.10.2...3.10.2)

project(cxxopts-download NONE)

include(ExternalProject)

ExternalProject_Add(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v2.2.0
    GIT_SHALLOW ON
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/cxxopts-src"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/cxxopts-build"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
