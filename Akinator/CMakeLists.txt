cmake_minimum_required(VERSION 3.6)
project(Akinator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES main.c tree.c tree.h)
add_executable(Akinator ${SOURCE_FILES})

add_subdirectory(tests)