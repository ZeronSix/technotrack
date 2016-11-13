cmake_minimum_required(VERSION 3.6)
project(ListsAndHashtables)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99")

set(LIST_SOURCES list.c list.h)
set(HASHMAP_SOURCES hashmap.c hashmap.h)

set(SOURCE_FILES main.c)
add_executable(ListsAndHashtables ${SOURCE_FILES} ${HASHMAP_SOURCES} ${LIST_SOURCES} )
add_subdirectory(tests)