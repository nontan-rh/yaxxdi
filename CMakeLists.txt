cmake_minimum_required(VERSION 3.10.2)
cmake_policy(VERSION 3.10.2...3.10.2)

project(yaxxdi CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/load_external_project.cmake)

yaxxdi_load_external_project(cmake/external_projects/cxxopts.cmake)

add_subdirectory(src)
