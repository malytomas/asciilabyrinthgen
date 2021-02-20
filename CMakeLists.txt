cmake_minimum_required(VERSION 3.10)
project(labgen CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/externals/cage/cmake")
include(cage_all)
cage_default_init()

add_subdirectory(externals/cage EXCLUDE_FROM_ALL)
add_subdirectory(sources)
