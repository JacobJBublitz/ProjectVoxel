cmake_minimum_required(VERSION 3.0)

project(ProjectVoxel)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

add_subdirectory(src)

add_executable(ProjectVoxel ${ProjectVoxel_SOURCES})
target_compile_definitions(ProjectVoxel PUBLIC ${ProjectVoxel_DEFINITIONS})
target_include_directories(ProjectVoxel PUBLIC SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/include" PUBLIC ${ProjectVoxel_INCLUDE_DIRS})
target_link_libraries(ProjectVoxel PUBLIC ${ProjectVoxel_LINK_LIBRARIES})
