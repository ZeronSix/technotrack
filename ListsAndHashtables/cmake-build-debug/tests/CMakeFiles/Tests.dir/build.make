# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2016.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2016.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zeronsix/projects/technotrack/ListsAndHashtables

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug

# Include any dependencies generated for this target.
include tests/CMakeFiles/Tests.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/Tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/Tests.dir/flags.make

tests/CMakeFiles/Tests.dir/tests.c.o: tests/CMakeFiles/Tests.dir/flags.make
tests/CMakeFiles/Tests.dir/tests.c.o: ../tests/tests.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/Tests.dir/tests.c.o"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Tests.dir/tests.c.o   -c /home/zeronsix/projects/technotrack/ListsAndHashtables/tests/tests.c

tests/CMakeFiles/Tests.dir/tests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Tests.dir/tests.c.i"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zeronsix/projects/technotrack/ListsAndHashtables/tests/tests.c > CMakeFiles/Tests.dir/tests.c.i

tests/CMakeFiles/Tests.dir/tests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Tests.dir/tests.c.s"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zeronsix/projects/technotrack/ListsAndHashtables/tests/tests.c -o CMakeFiles/Tests.dir/tests.c.s

tests/CMakeFiles/Tests.dir/tests.c.o.requires:

.PHONY : tests/CMakeFiles/Tests.dir/tests.c.o.requires

tests/CMakeFiles/Tests.dir/tests.c.o.provides: tests/CMakeFiles/Tests.dir/tests.c.o.requires
	$(MAKE) -f tests/CMakeFiles/Tests.dir/build.make tests/CMakeFiles/Tests.dir/tests.c.o.provides.build
.PHONY : tests/CMakeFiles/Tests.dir/tests.c.o.provides

tests/CMakeFiles/Tests.dir/tests.c.o.provides.build: tests/CMakeFiles/Tests.dir/tests.c.o


tests/CMakeFiles/Tests.dir/__/list.c.o: tests/CMakeFiles/Tests.dir/flags.make
tests/CMakeFiles/Tests.dir/__/list.c.o: ../list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/Tests.dir/__/list.c.o"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Tests.dir/__/list.c.o   -c /home/zeronsix/projects/technotrack/ListsAndHashtables/list.c

tests/CMakeFiles/Tests.dir/__/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Tests.dir/__/list.c.i"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zeronsix/projects/technotrack/ListsAndHashtables/list.c > CMakeFiles/Tests.dir/__/list.c.i

tests/CMakeFiles/Tests.dir/__/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Tests.dir/__/list.c.s"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zeronsix/projects/technotrack/ListsAndHashtables/list.c -o CMakeFiles/Tests.dir/__/list.c.s

tests/CMakeFiles/Tests.dir/__/list.c.o.requires:

.PHONY : tests/CMakeFiles/Tests.dir/__/list.c.o.requires

tests/CMakeFiles/Tests.dir/__/list.c.o.provides: tests/CMakeFiles/Tests.dir/__/list.c.o.requires
	$(MAKE) -f tests/CMakeFiles/Tests.dir/build.make tests/CMakeFiles/Tests.dir/__/list.c.o.provides.build
.PHONY : tests/CMakeFiles/Tests.dir/__/list.c.o.provides

tests/CMakeFiles/Tests.dir/__/list.c.o.provides.build: tests/CMakeFiles/Tests.dir/__/list.c.o


tests/CMakeFiles/Tests.dir/__/hashmap.c.o: tests/CMakeFiles/Tests.dir/flags.make
tests/CMakeFiles/Tests.dir/__/hashmap.c.o: ../hashmap.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object tests/CMakeFiles/Tests.dir/__/hashmap.c.o"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Tests.dir/__/hashmap.c.o   -c /home/zeronsix/projects/technotrack/ListsAndHashtables/hashmap.c

tests/CMakeFiles/Tests.dir/__/hashmap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Tests.dir/__/hashmap.c.i"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zeronsix/projects/technotrack/ListsAndHashtables/hashmap.c > CMakeFiles/Tests.dir/__/hashmap.c.i

tests/CMakeFiles/Tests.dir/__/hashmap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Tests.dir/__/hashmap.c.s"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zeronsix/projects/technotrack/ListsAndHashtables/hashmap.c -o CMakeFiles/Tests.dir/__/hashmap.c.s

tests/CMakeFiles/Tests.dir/__/hashmap.c.o.requires:

.PHONY : tests/CMakeFiles/Tests.dir/__/hashmap.c.o.requires

tests/CMakeFiles/Tests.dir/__/hashmap.c.o.provides: tests/CMakeFiles/Tests.dir/__/hashmap.c.o.requires
	$(MAKE) -f tests/CMakeFiles/Tests.dir/build.make tests/CMakeFiles/Tests.dir/__/hashmap.c.o.provides.build
.PHONY : tests/CMakeFiles/Tests.dir/__/hashmap.c.o.provides

tests/CMakeFiles/Tests.dir/__/hashmap.c.o.provides.build: tests/CMakeFiles/Tests.dir/__/hashmap.c.o


# Object files for target Tests
Tests_OBJECTS = \
"CMakeFiles/Tests.dir/tests.c.o" \
"CMakeFiles/Tests.dir/__/list.c.o" \
"CMakeFiles/Tests.dir/__/hashmap.c.o"

# External object files for target Tests
Tests_EXTERNAL_OBJECTS =

../Tests: tests/CMakeFiles/Tests.dir/tests.c.o
../Tests: tests/CMakeFiles/Tests.dir/__/list.c.o
../Tests: tests/CMakeFiles/Tests.dir/__/hashmap.c.o
../Tests: tests/CMakeFiles/Tests.dir/build.make
../Tests: tests/CMakeFiles/Tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable ../../Tests"
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/Tests.dir/build: ../Tests

.PHONY : tests/CMakeFiles/Tests.dir/build

tests/CMakeFiles/Tests.dir/requires: tests/CMakeFiles/Tests.dir/tests.c.o.requires
tests/CMakeFiles/Tests.dir/requires: tests/CMakeFiles/Tests.dir/__/list.c.o.requires
tests/CMakeFiles/Tests.dir/requires: tests/CMakeFiles/Tests.dir/__/hashmap.c.o.requires

.PHONY : tests/CMakeFiles/Tests.dir/requires

tests/CMakeFiles/Tests.dir/clean:
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests && $(CMAKE_COMMAND) -P CMakeFiles/Tests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/Tests.dir/clean

tests/CMakeFiles/Tests.dir/depend:
	cd /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zeronsix/projects/technotrack/ListsAndHashtables /home/zeronsix/projects/technotrack/ListsAndHashtables/tests /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests /home/zeronsix/projects/technotrack/ListsAndHashtables/cmake-build-debug/tests/CMakeFiles/Tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/Tests.dir/depend

