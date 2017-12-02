cmake_minimum_required (VERSION 2.6)
project (CERAII)

option(CERAII_TEST_BUILD "Test build with sanitizers" ON)
option(CERAII_CLANG_BUILD "Build with clang" OFF)
option(CERAII_PERF_OPTIMIZED_BUILD "Build with turned on optimizations" OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
include_directories(src)


set(CERAII_SOURCES src/ceraii.c)

add_executable(test_basic tests/basic.c ${CERAII_SOURCES})
add_executable(test_multithread tests/multi_threads.c ${CERAII_SOURCES})
target_link_libraries(test_multithread pthread)




if(CERAII_PERF_OPTIMIZED_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
else(CERAII_PERF_OPTIMIZED_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
endif(CERAII_PERF_OPTIMIZED_BUILD)


if(CERAII_TEST_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    if(NOT CERAII_CLANG_BUILD)
        target_link_libraries(test_multithread asan)
        target_link_libraries(test_basic asan)
    endif(NOT CERAII_CLANG_BUILD)
endif(CERAII_TEST_BUILD)
