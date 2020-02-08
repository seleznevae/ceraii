cmake_minimum_required (VERSION 3.0)

add_library(ceraii
    ceraii.c)

target_include_directories(ceraii
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
