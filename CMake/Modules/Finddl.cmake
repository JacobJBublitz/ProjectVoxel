# Finds libdl.
# Affected variables:
# - dl_FOUND: TRUE if libdl was found.
# - dl_INCLUDE_DIRS: Include path for libdl
# - dl_LIBRARIES: Libraries for libdl

find_path(dl_INCLUDE_DIRS dlfcn.h
        PATHS /usr/local/include /usr/include ${CMAKE_EXTRA_INCLUDES})

find_library(dl_LIBRARIES dl
        PATHS /usr/local/lib /usr/lib /lib ${CMAKE_EXTRA_LIBRARIES})

if (dl_INCLUDE_DIRS AND dl_LIBRARIES)
    set(dl_FOUND TRUE)
else ()
    if (NOT dl_FIND_QUIETLY)
        if (NOT dl_INCLUDE_DIRS)
            message(STATUS "Unable to find dl header files.")
        endif ()
        if (NOT dl_LIBRARIES)
            message(STATUS "Unable to find dl library files.")
        endif ()
    endif ()
endif ()

if (NOT dl_FOUND AND dl_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find dl!")
endif ()

mark_as_advanced(dl_FOUND dl_INCLUDE_DIRS dl_LIBRARIES)