cmake_minimum_required(VERSION 3.15)
project(tmp LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++-12)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")
#====================================================================
# set(OPTIMIZATION_FLAGS "-fno-elide-constructors")
# set(OPTIMIZATION_FLAGS "-fno-elide-constructors -DNO_MOVE")

# set(OPTIMIZATION_FLAGS "")
# set(OPTIMIZATION_FLAGS "-DNO_MOVE")
#====================================================================
set(O_LEVEL "-O1")

include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "${O_LEVEL} ${WARNING_FLAGS} ${OPTIMIZATION_FLAGS}") # -fsanitize=address

# find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
# find_package(OpenGL)

add_executable(prog
        # utils
        src/utils/timer.cpp

        src/utils/logger.cpp
        src/utils/tag.cpp

        src/utils/rgba/rgba.cpp

        src/utils/text_line/line.cpp
        src/utils/text_line/line_pos.cpp
        src/utils/text_line/cursor.cpp
        src/utils/text_line/line_history.cpp

        src/utils/state_history/history.cpp

        src/utils/micro_logger/func_logger.cpp

        # task

        src/main.cpp
)

add_custom_target(run
        COMMAND make
        COMMAND ./prog
        )

# target_link_libraries(moga sfml-graphics sfml-window sfml-system)
# target_link_libraries(moga ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})
