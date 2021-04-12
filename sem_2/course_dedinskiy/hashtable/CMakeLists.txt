cmake_minimum_required(VERSION 3.15)
project(zeta_path_tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_RELEASE  "-O0 ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "-g3 -O3 ${WARNING_FLAGS}")

add_executable(tabeletka
        src/main.cpp
        src/hashtable.h
        src/hashtable.cpp
        src/ht_node.h
        src/general/c/common.h)

