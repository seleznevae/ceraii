cmake_minimum_required (VERSION 2.6)
project (CERAII)

option(CERAII_TEST_BUILD "Test build with sanitizers and small library stack size" ON)
option(CERAII_MSVC_BUILD "Build with msvc" OFF)

option(CERAII_COVERALLS_BUILD "Build for coveralls" OFF)
option(CERAII_DO_BEFORE_EXIT "Doing actions before evaluating return expression" OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
include_directories(src)


set(CERAII_SOURCES src/ceraii.c)

add_executable(test_basic tests/basic.c ${CERAII_SOURCES})
add_executable(test_multithread tests/multi_threads.c ${CERAII_SOURCES})
if(NOT CERAII_MSVC_BUILD)
    target_link_libraries(test_multithread pthread)
endif(NOT CERAII_MSVC_BUILD)

add_executable(example examples/example.c ${CERAII_SOURCES})

message(STATUS "CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

if(CERAII_TEST_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    add_definitions(-DCERAII_CONF_ENV_STACK_SIZE=2)
    add_definitions(-DCERAII_CONF_ALLOC_ENV_STACK_INITIAL_CAPACITY=10)
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(test_multithread asan)
        target_link_libraries(test_basic asan)
    endif()
endif(CERAII_TEST_BUILD)



if(CERAII_COVERALLS_BUILD) #for coveralls
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
endif(CERAII_COVERALLS_BUILD)

if(CERAII_DO_BEFORE_EXIT)
    add_definitions(-DCERAII_ACTIONS_BEFORE_RETURN_EXPRESSION)
endif(CERAII_DO_BEFORE_EXIT)



