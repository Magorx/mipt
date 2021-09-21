cmake_minimum_required(VERSION 3.15)
project(zeta_path_tracer LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O3 ${WARNING_FLAGS}")

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)

add_executable(partcls
        src/main.cpp
        src/graphics/context.cpp)

target_link_libraries(partcls sfml-graphics sfml-window sfml-system)        

add_custom_target(convert
        COMMAND convert image.ppm image.png
        )

add_custom_target(view
        COMMAND eog -f image.png
        )

