# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/Tools/fireflow

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/Tools/fireflow/build

# Include any dependencies generated for this target.
include CMakeFiles/fireflow_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fireflow_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fireflow_test.dir/flags.make

CMakeFiles/fireflow_test.dir/src/capture.cpp.o: CMakeFiles/fireflow_test.dir/flags.make
CMakeFiles/fireflow_test.dir/src/capture.cpp.o: ../src/capture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fireflow_test.dir/src/capture.cpp.o"
	/usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fireflow_test.dir/src/capture.cpp.o -c /home/ubuntu/Tools/fireflow/src/capture.cpp

CMakeFiles/fireflow_test.dir/src/capture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fireflow_test.dir/src/capture.cpp.i"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Tools/fireflow/src/capture.cpp > CMakeFiles/fireflow_test.dir/src/capture.cpp.i

CMakeFiles/fireflow_test.dir/src/capture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fireflow_test.dir/src/capture.cpp.s"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Tools/fireflow/src/capture.cpp -o CMakeFiles/fireflow_test.dir/src/capture.cpp.s

CMakeFiles/fireflow_test.dir/src/capture.cpp.o.requires:

.PHONY : CMakeFiles/fireflow_test.dir/src/capture.cpp.o.requires

CMakeFiles/fireflow_test.dir/src/capture.cpp.o.provides: CMakeFiles/fireflow_test.dir/src/capture.cpp.o.requires
	$(MAKE) -f CMakeFiles/fireflow_test.dir/build.make CMakeFiles/fireflow_test.dir/src/capture.cpp.o.provides.build
.PHONY : CMakeFiles/fireflow_test.dir/src/capture.cpp.o.provides

CMakeFiles/fireflow_test.dir/src/capture.cpp.o.provides.build: CMakeFiles/fireflow_test.dir/src/capture.cpp.o


CMakeFiles/fireflow_test.dir/src/cusum.cpp.o: CMakeFiles/fireflow_test.dir/flags.make
CMakeFiles/fireflow_test.dir/src/cusum.cpp.o: ../src/cusum.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fireflow_test.dir/src/cusum.cpp.o"
	/usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fireflow_test.dir/src/cusum.cpp.o -c /home/ubuntu/Tools/fireflow/src/cusum.cpp

CMakeFiles/fireflow_test.dir/src/cusum.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fireflow_test.dir/src/cusum.cpp.i"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Tools/fireflow/src/cusum.cpp > CMakeFiles/fireflow_test.dir/src/cusum.cpp.i

CMakeFiles/fireflow_test.dir/src/cusum.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fireflow_test.dir/src/cusum.cpp.s"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Tools/fireflow/src/cusum.cpp -o CMakeFiles/fireflow_test.dir/src/cusum.cpp.s

CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.requires:

.PHONY : CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.requires

CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.provides: CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.requires
	$(MAKE) -f CMakeFiles/fireflow_test.dir/build.make CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.provides.build
.PHONY : CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.provides

CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.provides.build: CMakeFiles/fireflow_test.dir/src/cusum.cpp.o


CMakeFiles/fireflow_test.dir/src/detector.cpp.o: CMakeFiles/fireflow_test.dir/flags.make
CMakeFiles/fireflow_test.dir/src/detector.cpp.o: ../src/detector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fireflow_test.dir/src/detector.cpp.o"
	/usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fireflow_test.dir/src/detector.cpp.o -c /home/ubuntu/Tools/fireflow/src/detector.cpp

CMakeFiles/fireflow_test.dir/src/detector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fireflow_test.dir/src/detector.cpp.i"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Tools/fireflow/src/detector.cpp > CMakeFiles/fireflow_test.dir/src/detector.cpp.i

CMakeFiles/fireflow_test.dir/src/detector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fireflow_test.dir/src/detector.cpp.s"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Tools/fireflow/src/detector.cpp -o CMakeFiles/fireflow_test.dir/src/detector.cpp.s

CMakeFiles/fireflow_test.dir/src/detector.cpp.o.requires:

.PHONY : CMakeFiles/fireflow_test.dir/src/detector.cpp.o.requires

CMakeFiles/fireflow_test.dir/src/detector.cpp.o.provides: CMakeFiles/fireflow_test.dir/src/detector.cpp.o.requires
	$(MAKE) -f CMakeFiles/fireflow_test.dir/build.make CMakeFiles/fireflow_test.dir/src/detector.cpp.o.provides.build
.PHONY : CMakeFiles/fireflow_test.dir/src/detector.cpp.o.provides

CMakeFiles/fireflow_test.dir/src/detector.cpp.o.provides.build: CMakeFiles/fireflow_test.dir/src/detector.cpp.o


CMakeFiles/fireflow_test.dir/src/entropy.cpp.o: CMakeFiles/fireflow_test.dir/flags.make
CMakeFiles/fireflow_test.dir/src/entropy.cpp.o: ../src/entropy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/fireflow_test.dir/src/entropy.cpp.o"
	/usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fireflow_test.dir/src/entropy.cpp.o -c /home/ubuntu/Tools/fireflow/src/entropy.cpp

CMakeFiles/fireflow_test.dir/src/entropy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fireflow_test.dir/src/entropy.cpp.i"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Tools/fireflow/src/entropy.cpp > CMakeFiles/fireflow_test.dir/src/entropy.cpp.i

CMakeFiles/fireflow_test.dir/src/entropy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fireflow_test.dir/src/entropy.cpp.s"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Tools/fireflow/src/entropy.cpp -o CMakeFiles/fireflow_test.dir/src/entropy.cpp.s

CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.requires:

.PHONY : CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.requires

CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.provides: CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.requires
	$(MAKE) -f CMakeFiles/fireflow_test.dir/build.make CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.provides.build
.PHONY : CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.provides

CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.provides.build: CMakeFiles/fireflow_test.dir/src/entropy.cpp.o


CMakeFiles/fireflow_test.dir/src/mapping.cpp.o: CMakeFiles/fireflow_test.dir/flags.make
CMakeFiles/fireflow_test.dir/src/mapping.cpp.o: ../src/mapping.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/fireflow_test.dir/src/mapping.cpp.o"
	/usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fireflow_test.dir/src/mapping.cpp.o -c /home/ubuntu/Tools/fireflow/src/mapping.cpp

CMakeFiles/fireflow_test.dir/src/mapping.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fireflow_test.dir/src/mapping.cpp.i"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Tools/fireflow/src/mapping.cpp > CMakeFiles/fireflow_test.dir/src/mapping.cpp.i

CMakeFiles/fireflow_test.dir/src/mapping.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fireflow_test.dir/src/mapping.cpp.s"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Tools/fireflow/src/mapping.cpp -o CMakeFiles/fireflow_test.dir/src/mapping.cpp.s

CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.requires:

.PHONY : CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.requires

CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.provides: CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.requires
	$(MAKE) -f CMakeFiles/fireflow_test.dir/build.make CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.provides.build
.PHONY : CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.provides

CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.provides.build: CMakeFiles/fireflow_test.dir/src/mapping.cpp.o


CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o: CMakeFiles/fireflow_test.dir/flags.make
CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o: ../test/fireflow_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o"
	/usr/bin/g++-7  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o -c /home/ubuntu/Tools/fireflow/test/fireflow_test.cpp

CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.i"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Tools/fireflow/test/fireflow_test.cpp > CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.i

CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.s"
	/usr/bin/g++-7 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Tools/fireflow/test/fireflow_test.cpp -o CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.s

CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.requires:

.PHONY : CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.requires

CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.provides: CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.requires
	$(MAKE) -f CMakeFiles/fireflow_test.dir/build.make CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.provides.build
.PHONY : CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.provides

CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.provides.build: CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o


# Object files for target fireflow_test
fireflow_test_OBJECTS = \
"CMakeFiles/fireflow_test.dir/src/capture.cpp.o" \
"CMakeFiles/fireflow_test.dir/src/cusum.cpp.o" \
"CMakeFiles/fireflow_test.dir/src/detector.cpp.o" \
"CMakeFiles/fireflow_test.dir/src/entropy.cpp.o" \
"CMakeFiles/fireflow_test.dir/src/mapping.cpp.o" \
"CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o"

# External object files for target fireflow_test
fireflow_test_EXTERNAL_OBJECTS =

fireflow_test: CMakeFiles/fireflow_test.dir/src/capture.cpp.o
fireflow_test: CMakeFiles/fireflow_test.dir/src/cusum.cpp.o
fireflow_test: CMakeFiles/fireflow_test.dir/src/detector.cpp.o
fireflow_test: CMakeFiles/fireflow_test.dir/src/entropy.cpp.o
fireflow_test: CMakeFiles/fireflow_test.dir/src/mapping.cpp.o
fireflow_test: CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o
fireflow_test: CMakeFiles/fireflow_test.dir/build.make
fireflow_test: /usr/local/lib/libgtest.a
fireflow_test: CMakeFiles/fireflow_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/Tools/fireflow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable fireflow_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fireflow_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fireflow_test.dir/build: fireflow_test

.PHONY : CMakeFiles/fireflow_test.dir/build

CMakeFiles/fireflow_test.dir/requires: CMakeFiles/fireflow_test.dir/src/capture.cpp.o.requires
CMakeFiles/fireflow_test.dir/requires: CMakeFiles/fireflow_test.dir/src/cusum.cpp.o.requires
CMakeFiles/fireflow_test.dir/requires: CMakeFiles/fireflow_test.dir/src/detector.cpp.o.requires
CMakeFiles/fireflow_test.dir/requires: CMakeFiles/fireflow_test.dir/src/entropy.cpp.o.requires
CMakeFiles/fireflow_test.dir/requires: CMakeFiles/fireflow_test.dir/src/mapping.cpp.o.requires
CMakeFiles/fireflow_test.dir/requires: CMakeFiles/fireflow_test.dir/test/fireflow_test.cpp.o.requires

.PHONY : CMakeFiles/fireflow_test.dir/requires

CMakeFiles/fireflow_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fireflow_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fireflow_test.dir/clean

CMakeFiles/fireflow_test.dir/depend:
	cd /home/ubuntu/Tools/fireflow/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Tools/fireflow /home/ubuntu/Tools/fireflow /home/ubuntu/Tools/fireflow/build /home/ubuntu/Tools/fireflow/build /home/ubuntu/Tools/fireflow/build/CMakeFiles/fireflow_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fireflow_test.dir/depend
