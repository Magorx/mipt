# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/kctf/Repositories/moga_engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kctf/Repositories/moga_engine/build

# Include any dependencies generated for this target.
include CMakeFiles/rainbow_stamp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rainbow_stamp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rainbow_stamp.dir/flags.make

CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.o: CMakeFiles/rainbow_stamp.dir/flags.make
CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.o: ../src/collections/plugins/rainbow_stamp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/moga_engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.o"
	clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.o -c /home/kctf/Repositories/moga_engine/src/collections/plugins/rainbow_stamp.cpp

CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.i"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/moga_engine/src/collections/plugins/rainbow_stamp.cpp > CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.i

CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.s"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/moga_engine/src/collections/plugins/rainbow_stamp.cpp -o CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.s

# Object files for target rainbow_stamp
rainbow_stamp_OBJECTS = \
"CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.o"

# External object files for target rainbow_stamp
rainbow_stamp_EXTERNAL_OBJECTS =

resources/plugin/tool/kctf_rainbow_stamp.so: CMakeFiles/rainbow_stamp.dir/src/collections/plugins/rainbow_stamp.cpp.o
resources/plugin/tool/kctf_rainbow_stamp.so: CMakeFiles/rainbow_stamp.dir/build.make
resources/plugin/tool/kctf_rainbow_stamp.so: CMakeFiles/rainbow_stamp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kctf/Repositories/moga_engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library resources/plugin/tool/kctf_rainbow_stamp.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rainbow_stamp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rainbow_stamp.dir/build: resources/plugin/tool/kctf_rainbow_stamp.so

.PHONY : CMakeFiles/rainbow_stamp.dir/build

CMakeFiles/rainbow_stamp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rainbow_stamp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rainbow_stamp.dir/clean

CMakeFiles/rainbow_stamp.dir/depend:
	cd /home/kctf/Repositories/moga_engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine/build /home/kctf/Repositories/moga_engine/build /home/kctf/Repositories/moga_engine/build/CMakeFiles/rainbow_stamp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rainbow_stamp.dir/depend
