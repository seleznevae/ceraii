cmake_minimum_required (VERSION 3.0)
project (CERAII)

option(CERAII_TEST_BUILD "Test build with sanitizers and small library stack size" OFF)
option(CERAII_COVERALLS_BUILD "Build for coveralls" OFF)
option(CERAII_DO_BEFORE_EXIT "Doing actions before evaluating return expression" OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(CERAII_DO_BEFORE_EXIT)
    add_definitions(-DCERAII_ACTIONS_BEFORE_RETURN_EXPRESSION)
endif()

add_subdirectory(src)

add_executable(test_basic
    tests/basic.c)
target_link_libraries(test_basic PRIVATE ceraii)

add_executable(test_multithread
    tests/multi_threads.c)
target_link_libraries(test_multithread PRIVATE ceraii $<$<NOT:$<C_COMPILER_ID:MSVC>>:pthread> )

add_executable(example
    examples/example.c)
target_link_libraries(example PRIVATE ceraii)


if(CERAII_TEST_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    add_definitions(-DCERAII_CONF_ENV_STACK_SIZE=2)
    add_definitions(-DCERAII_CONF_ALLOC_ENV_STACK_INITIAL_CAPACITY=10)
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(test_multithread PRIVATE asan)
        target_link_libraries(test_basic PRIVATE asan)
    endif()
endif()

if(CERAII_COVERALLS_BUILD) #for coveralls
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
endif()





