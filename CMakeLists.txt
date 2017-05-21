cmake_minimum_required(VERSION 3.0)

project(ProjectVoxel)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Libraries/glfw)

list(APPEND ProjectVoxel_LINK_LIBRARIES glfw)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

add_subdirectory(src)

add_executable(ProjectVoxel ${ProjectVoxel_SOURCES})
set_target_properties(ProjectVoxel PROPERTIES CXX_STANDARD 14)
target_compile_definitions(ProjectVoxel PUBLIC ${ProjectVoxel_DEFINITIONS})
target_include_directories(ProjectVoxel PUBLIC ${ProjectVoxel_INCLUDE_DIRS})
target_link_libraries(ProjectVoxel ${ProjectVoxel_LINK_LIBRARIES})
