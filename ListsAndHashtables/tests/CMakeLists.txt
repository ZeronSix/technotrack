include_directories(../)

set(TEST_SOURCE_FILES tests.c)
add_executable(Tests ${TEST_SOURCE_FILES} ../list.c ../hashmap.c)