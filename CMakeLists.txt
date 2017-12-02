cmake_minimum_required (VERSION 2.6)
project (CERAII)

set(CERAII_SOURCES src/ceraii.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

include_directories(src)

add_executable(test_basic tests/basic.c ${CERAII_SOURCES})


add_executable(test_multithread tests/multi_threads.c ${CERAII_SOURCES})
target_link_libraries(test_multithread pthread)

