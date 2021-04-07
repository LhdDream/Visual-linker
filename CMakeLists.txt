cmake_minimum_required(VERSION 3.2)
project(Visual-linker)


set(CMAKE_CXX_STANDARD 17)

include_directories(src)

aux_source_directory(src SOURCE_SRC)


add_definitions("--std=c++17")
add_definitions("-g")
add_definitions("-O3")

add_executable(Visual-linker main.cpp ${SOURCE_SRC}
        )