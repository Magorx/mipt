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
include CMakeFiles/invasion.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/invasion.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/invasion.dir/flags.make

CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.o: CMakeFiles/invasion.dir/flags.make
CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.o: ../src/collections/plugins/invasion.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/moga_engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.o"
	clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.o -c /home/kctf/Repositories/moga_engine/src/collections/plugins/invasion.cpp

CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.i"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/moga_engine/src/collections/plugins/invasion.cpp > CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.i

CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.s"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/moga_engine/src/collections/plugins/invasion.cpp -o CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.s

# Object files for target invasion
invasion_OBJECTS = \
"CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.o"

# External object files for target invasion
invasion_EXTERNAL_OBJECTS =

resources/plugin/extension/invasion.so: CMakeFiles/invasion.dir/src/collections/plugins/invasion.cpp.o
resources/plugin/extension/invasion.so: CMakeFiles/invasion.dir/build.make
resources/plugin/extension/invasion.so: CMakeFiles/invasion.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kctf/Repositories/moga_engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library resources/plugin/extension/invasion.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/invasion.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/invasion.dir/build: resources/plugin/extension/invasion.so

.PHONY : CMakeFiles/invasion.dir/build

CMakeFiles/invasion.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/invasion.dir/cmake_clean.cmake
.PHONY : CMakeFiles/invasion.dir/clean

CMakeFiles/invasion.dir/depend:
	cd /home/kctf/Repositories/moga_engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine/build /home/kctf/Repositories/moga_engine/build /home/kctf/Repositories/moga_engine/build/CMakeFiles/invasion.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/invasion.dir/depend
