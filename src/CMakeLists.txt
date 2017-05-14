macro(add_definition DEFINITION)
    list(APPEND ProjectVoxel_DEFINITIONS ${DEFINITION})
endmacro()

macro(add_include_dir INCLUDE_DIR)
    list(APPEND ProjectVoxel_INCLUDE_DIRS ${INCLUDE_DIR})
endmacro()

macro(add_link_library LIBRARY)
    list(APPEND ProjectVoxel_LINK_LIBRARIES ${LIBRARY})
endmacro()

macro(add_source SOURCE)
    foreach (ext c cpp h hpp)
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}.${ext}")
            list(APPEND ProjectVoxel_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}.${ext}")
        endif ()

        foreach (platform ${_platforms})
            if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}_${platform}.${ext}")
                list(APPEND ProjectVoxel_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}_${platform}.${ext}")
            endif ()
        endforeach ()
    endforeach ()
endmacro()

macro(update_parent)
    set(ProjectVoxel_DEFINITIONS ${ProjectVoxel_DEFINITIONS} PARENT_SCOPE)
    set(ProjectVoxel_INCLUDE_DIRS ${ProjectVoxel_INCLUDE_DIRS} PARENT_SCOPE)
    set(ProjectVoxel_LINK_LIBRARIES ${ProjectVoxel_LINK_LIBRARIES} PARENT_SCOPE)
    set(ProjectVoxel_SOURCES ${ProjectVoxel_SOURCES} PARENT_SCOPE)
endmacro()

set(_platforms OpenGL Vulkan)
if (WIN32)
    list(APPEND _platforms WinAPI)
else ()
    list(APPEND _platforms POSIX)

    if (APPLE)
        list(APPEND _platforms macOS)
    endif ()
endif ()

find_package(XCB)
if (XCB_FOUND)
    add_include_dir(${XCB_INCLUDE_DIRS})
    add_link_library(${XCB_LIBRARIES})
    add_definition("ProjectVoxel_HAS_XCB")
    list(APPEND _platforms XCB)
endif ()

add_subdirectory(Graphics)
add_subdirectory(IO)

foreach (platform ${_platforms})
    add_source(${platform})
endforeach ()

add_source(Main)

update_parent()