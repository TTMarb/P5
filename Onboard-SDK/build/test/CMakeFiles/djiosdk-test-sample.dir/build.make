# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/Documents/P5/Onboard-SDK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/Documents/P5/Onboard-SDK/build

# Include any dependencies generated for this target.
include test/CMakeFiles/djiosdk-test-sample.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/djiosdk-test-sample.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/djiosdk-test-sample.dir/flags.make

test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o: test/CMakeFiles/djiosdk-test-sample.dir/flags.make
test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o: ../test/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ubuntu/Documents/P5/Onboard-SDK/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/djiosdk-test-sample.dir/main.cpp.o -c /home/ubuntu/Documents/P5/Onboard-SDK/test/main.cpp

test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/djiosdk-test-sample.dir/main.cpp.i"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ubuntu/Documents/P5/Onboard-SDK/test/main.cpp > CMakeFiles/djiosdk-test-sample.dir/main.cpp.i

test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/djiosdk-test-sample.dir/main.cpp.s"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ubuntu/Documents/P5/Onboard-SDK/test/main.cpp -o CMakeFiles/djiosdk-test-sample.dir/main.cpp.s

test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.requires:
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.requires

test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.provides: test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/djiosdk-test-sample.dir/build.make test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.provides.build
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.provides

test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.provides.build: test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o

test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o: test/CMakeFiles/djiosdk-test-sample.dir/flags.make
test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o: ../test/test_project.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ubuntu/Documents/P5/Onboard-SDK/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o -c /home/ubuntu/Documents/P5/Onboard-SDK/test/test_project.cpp

test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.i"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ubuntu/Documents/P5/Onboard-SDK/test/test_project.cpp > CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.i

test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.s"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ubuntu/Documents/P5/Onboard-SDK/test/test_project.cpp -o CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.s

test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.requires:
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.requires

test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.provides: test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/djiosdk-test-sample.dir/build.make test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.provides.build
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.provides

test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.provides.build: test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o

# Object files for target djiosdk-test-sample
djiosdk__test__sample_OBJECTS = \
"CMakeFiles/djiosdk-test-sample.dir/main.cpp.o" \
"CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o"

# External object files for target djiosdk-test-sample
djiosdk__test__sample_EXTERNAL_OBJECTS =

bin/djiosdk-test-sample: test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o
bin/djiosdk-test-sample: test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o
bin/djiosdk-test-sample: test/CMakeFiles/djiosdk-test-sample.dir/build.make
bin/djiosdk-test-sample: libs/libdjiosdk-core.a
bin/djiosdk-test-sample: test/CMakeFiles/djiosdk-test-sample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/djiosdk-test-sample"
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/djiosdk-test-sample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/djiosdk-test-sample.dir/build: bin/djiosdk-test-sample
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/build

test/CMakeFiles/djiosdk-test-sample.dir/requires: test/CMakeFiles/djiosdk-test-sample.dir/main.cpp.o.requires
test/CMakeFiles/djiosdk-test-sample.dir/requires: test/CMakeFiles/djiosdk-test-sample.dir/test_project.cpp.o.requires
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/requires

test/CMakeFiles/djiosdk-test-sample.dir/clean:
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build/test && $(CMAKE_COMMAND) -P CMakeFiles/djiosdk-test-sample.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/clean

test/CMakeFiles/djiosdk-test-sample.dir/depend:
	cd /home/ubuntu/Documents/P5/Onboard-SDK/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Documents/P5/Onboard-SDK /home/ubuntu/Documents/P5/Onboard-SDK/test /home/ubuntu/Documents/P5/Onboard-SDK/build /home/ubuntu/Documents/P5/Onboard-SDK/build/test /home/ubuntu/Documents/P5/Onboard-SDK/build/test/CMakeFiles/djiosdk-test-sample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/djiosdk-test-sample.dir/depend

