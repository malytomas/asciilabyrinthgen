cmake_minimum_required(VERSION 3.10)

message(STATUS "----------------------------------------------------------------")
message(STATUS "labgen")
message(STATUS "----------------------------------------------------------------")

file(GLOB_RECURSE labgen-sources "labgen/*")
add_executable(labgen ${labgen-sources})
target_link_libraries(labgen cage-core)
cage_ide_sort_files(labgen)
cage_ide_working_dir_in_place(labgen)
