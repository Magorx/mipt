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
CMAKE_BINARY_DIR = /home/kctf/Repositories/moga_engine

# Include any dependencies generated for this target.
include CMakeFiles/Brush.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Brush.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Brush.dir/flags.make

CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.o: CMakeFiles/Brush.dir/flags.make
CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.o: src/collections/plugins/brush.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/moga_engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.o"
	clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.o -c /home/kctf/Repositories/moga_engine/src/collections/plugins/brush.cpp

CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.i"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/moga_engine/src/collections/plugins/brush.cpp > CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.i

CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.s"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/moga_engine/src/collections/plugins/brush.cpp -o CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.s

# Object files for target Brush
Brush_OBJECTS = \
"CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.o"

# External object files for target Brush
Brush_EXTERNAL_OBJECTS =

libBrush.so: CMakeFiles/Brush.dir/src/collections/plugins/brush.cpp.o
libBrush.so: CMakeFiles/Brush.dir/build.make
libBrush.so: CMakeFiles/Brush.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kctf/Repositories/moga_engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libBrush.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Brush.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Brush.dir/build: libBrush.so

.PHONY : CMakeFiles/Brush.dir/build

CMakeFiles/Brush.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Brush.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Brush.dir/clean

CMakeFiles/Brush.dir/depend:
	cd /home/kctf/Repositories/moga_engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine /home/kctf/Repositories/moga_engine/CMakeFiles/Brush.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Brush.dir/depend
