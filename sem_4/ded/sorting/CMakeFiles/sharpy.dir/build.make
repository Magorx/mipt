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
CMAKE_SOURCE_DIR = /home/kctf/Repositories/mipt/sem_4/ded/sorting

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kctf/Repositories/mipt/sem_4/ded/sorting

# Include any dependencies generated for this target.
include CMakeFiles/sharpy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sharpy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sharpy.dir/flags.make

CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.o: CMakeFiles/sharpy.dir/flags.make
CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.o: src/collections/plugins/sharpy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/sorting/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.o"
	clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/sorting/src/collections/plugins/sharpy.cpp

CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.i"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/sorting/src/collections/plugins/sharpy.cpp > CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.i

CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.s"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/sorting/src/collections/plugins/sharpy.cpp -o CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.s

# Object files for target sharpy
sharpy_OBJECTS = \
"CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.o"

# External object files for target sharpy
sharpy_EXTERNAL_OBJECTS =

resources/plugin/tool/kctf_sharpy.so: CMakeFiles/sharpy.dir/src/collections/plugins/sharpy.cpp.o
resources/plugin/tool/kctf_sharpy.so: CMakeFiles/sharpy.dir/build.make
resources/plugin/tool/kctf_sharpy.so: CMakeFiles/sharpy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/sorting/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library resources/plugin/tool/kctf_sharpy.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sharpy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sharpy.dir/build: resources/plugin/tool/kctf_sharpy.so

.PHONY : CMakeFiles/sharpy.dir/build

CMakeFiles/sharpy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sharpy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sharpy.dir/clean

CMakeFiles/sharpy.dir/depend:
	cd /home/kctf/Repositories/mipt/sem_4/ded/sorting && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kctf/Repositories/mipt/sem_4/ded/sorting /home/kctf/Repositories/mipt/sem_4/ded/sorting /home/kctf/Repositories/mipt/sem_4/ded/sorting /home/kctf/Repositories/mipt/sem_4/ded/sorting /home/kctf/Repositories/mipt/sem_4/ded/sorting/CMakeFiles/sharpy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sharpy.dir/depend

