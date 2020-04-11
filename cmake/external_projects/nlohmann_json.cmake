cmake_minimum_required(VERSION 3.10.2)
cmake_policy(VERSION 3.10.2...3.10.2)

project(nlohmann_json-download NONE)

include(ExternalProject)

ExternalProject_Add(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.7.3
    GIT_SHALLOW ON
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json-src"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json-build"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
