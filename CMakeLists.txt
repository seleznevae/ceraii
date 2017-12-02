cmake_minimum_required (VERSION 2.6)
project (CERAII)

option(CERAII_TEST_BUILD "Test build with turned off optimizations and with sanitizers" ON)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall --std=c11")
include_directories(src)


set(CERAII_SOURCES src/ceraii.c)

add_executable(test_basic tests/basic.c ${CERAII_SOURCES})
add_executable(test_multithread tests/multi_threads.c ${CERAII_SOURCES})
target_link_libraries(test_multithread pthread)




if(CERAII_TEST_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    target_link_libraries(test_multithread asan)
    target_link_libraries(test_basic asan)
endif(CERAII_TEST_BUILD)
