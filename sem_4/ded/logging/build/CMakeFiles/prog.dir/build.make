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
CMAKE_SOURCE_DIR = /home/kctf/Repositories/mipt/sem_4/ded/logging

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kctf/Repositories/mipt/sem_4/ded/logging/build

# Include any dependencies generated for this target.
include CMakeFiles/prog.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/prog.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/prog.dir/flags.make

CMakeFiles/prog.dir/src/utils/timer.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/timer.cpp.o: ../src/utils/timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/prog.dir/src/utils/timer.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/timer.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/timer.cpp

CMakeFiles/prog.dir/src/utils/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/timer.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/timer.cpp > CMakeFiles/prog.dir/src/utils/timer.cpp.i

CMakeFiles/prog.dir/src/utils/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/timer.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/timer.cpp -o CMakeFiles/prog.dir/src/utils/timer.cpp.s

CMakeFiles/prog.dir/src/utils/logger.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/logger.cpp.o: ../src/utils/logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/prog.dir/src/utils/logger.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/logger.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/logger.cpp

CMakeFiles/prog.dir/src/utils/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/logger.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/logger.cpp > CMakeFiles/prog.dir/src/utils/logger.cpp.i

CMakeFiles/prog.dir/src/utils/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/logger.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/logger.cpp -o CMakeFiles/prog.dir/src/utils/logger.cpp.s

CMakeFiles/prog.dir/src/utils/tag.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/tag.cpp.o: ../src/utils/tag.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/prog.dir/src/utils/tag.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/tag.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/tag.cpp

CMakeFiles/prog.dir/src/utils/tag.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/tag.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/tag.cpp > CMakeFiles/prog.dir/src/utils/tag.cpp.i

CMakeFiles/prog.dir/src/utils/tag.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/tag.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/tag.cpp -o CMakeFiles/prog.dir/src/utils/tag.cpp.s

CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.o: ../src/utils/rgba/rgba.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/rgba/rgba.cpp

CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/rgba/rgba.cpp > CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.i

CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/rgba/rgba.cpp -o CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.s

CMakeFiles/prog.dir/src/utils/text_line/line.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/text_line/line.cpp.o: ../src/utils/text_line/line.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/prog.dir/src/utils/text_line/line.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/text_line/line.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line.cpp

CMakeFiles/prog.dir/src/utils/text_line/line.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/text_line/line.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line.cpp > CMakeFiles/prog.dir/src/utils/text_line/line.cpp.i

CMakeFiles/prog.dir/src/utils/text_line/line.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/text_line/line.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line.cpp -o CMakeFiles/prog.dir/src/utils/text_line/line.cpp.s

CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.o: ../src/utils/text_line/line_pos.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line_pos.cpp

CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line_pos.cpp > CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.i

CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line_pos.cpp -o CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.s

CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.o: ../src/utils/text_line/cursor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/cursor.cpp

CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/cursor.cpp > CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.i

CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/cursor.cpp -o CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.s

CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.o: ../src/utils/text_line/line_history.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line_history.cpp

CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line_history.cpp > CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.i

CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/text_line/line_history.cpp -o CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.s

CMakeFiles/prog.dir/src/utils/state_history/history.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/utils/state_history/history.cpp.o: ../src/utils/state_history/history.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/prog.dir/src/utils/state_history/history.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/utils/state_history/history.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/state_history/history.cpp

CMakeFiles/prog.dir/src/utils/state_history/history.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/utils/state_history/history.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/state_history/history.cpp > CMakeFiles/prog.dir/src/utils/state_history/history.cpp.i

CMakeFiles/prog.dir/src/utils/state_history/history.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/utils/state_history/history.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/utils/state_history/history.cpp -o CMakeFiles/prog.dir/src/utils/state_history/history.cpp.s

CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.o: ../src/micro_logger/func_logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/micro_logger/func_logger.cpp

CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/micro_logger/func_logger.cpp > CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.i

CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/micro_logger/func_logger.cpp -o CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.s

CMakeFiles/prog.dir/src/main.cpp.o: CMakeFiles/prog.dir/flags.make
CMakeFiles/prog.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/prog.dir/src/main.cpp.o"
	g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/prog.dir/src/main.cpp.o -c /home/kctf/Repositories/mipt/sem_4/ded/logging/src/main.cpp

CMakeFiles/prog.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/prog.dir/src/main.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kctf/Repositories/mipt/sem_4/ded/logging/src/main.cpp > CMakeFiles/prog.dir/src/main.cpp.i

CMakeFiles/prog.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/prog.dir/src/main.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kctf/Repositories/mipt/sem_4/ded/logging/src/main.cpp -o CMakeFiles/prog.dir/src/main.cpp.s

# Object files for target prog
prog_OBJECTS = \
"CMakeFiles/prog.dir/src/utils/timer.cpp.o" \
"CMakeFiles/prog.dir/src/utils/logger.cpp.o" \
"CMakeFiles/prog.dir/src/utils/tag.cpp.o" \
"CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.o" \
"CMakeFiles/prog.dir/src/utils/text_line/line.cpp.o" \
"CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.o" \
"CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.o" \
"CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.o" \
"CMakeFiles/prog.dir/src/utils/state_history/history.cpp.o" \
"CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.o" \
"CMakeFiles/prog.dir/src/main.cpp.o"

# External object files for target prog
prog_EXTERNAL_OBJECTS =

prog: CMakeFiles/prog.dir/src/utils/timer.cpp.o
prog: CMakeFiles/prog.dir/src/utils/logger.cpp.o
prog: CMakeFiles/prog.dir/src/utils/tag.cpp.o
prog: CMakeFiles/prog.dir/src/utils/rgba/rgba.cpp.o
prog: CMakeFiles/prog.dir/src/utils/text_line/line.cpp.o
prog: CMakeFiles/prog.dir/src/utils/text_line/line_pos.cpp.o
prog: CMakeFiles/prog.dir/src/utils/text_line/cursor.cpp.o
prog: CMakeFiles/prog.dir/src/utils/text_line/line_history.cpp.o
prog: CMakeFiles/prog.dir/src/utils/state_history/history.cpp.o
prog: CMakeFiles/prog.dir/src/micro_logger/func_logger.cpp.o
prog: CMakeFiles/prog.dir/src/main.cpp.o
prog: CMakeFiles/prog.dir/build.make
prog: CMakeFiles/prog.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable prog"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/prog.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/prog.dir/build: prog

.PHONY : CMakeFiles/prog.dir/build

CMakeFiles/prog.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/prog.dir/cmake_clean.cmake
.PHONY : CMakeFiles/prog.dir/clean

CMakeFiles/prog.dir/depend:
	cd /home/kctf/Repositories/mipt/sem_4/ded/logging/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kctf/Repositories/mipt/sem_4/ded/logging /home/kctf/Repositories/mipt/sem_4/ded/logging /home/kctf/Repositories/mipt/sem_4/ded/logging/build /home/kctf/Repositories/mipt/sem_4/ded/logging/build /home/kctf/Repositories/mipt/sem_4/ded/logging/build/CMakeFiles/prog.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/prog.dir/depend

