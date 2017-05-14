if (UNIX OR APPLE)
    find_package(dl REQUIRED)
    add_include_dir(${dl_INCLUDE_DIRS})
    add_link_library(${dl_LIBRARIES})
endif ()

add_source(Library)

update_parent()