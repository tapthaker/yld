cmake_minimum_required(VERSION 3.16)
project(yld)

set(CMAKE_CXX_STANDARD 14)

add_library(elfio INTERFACE)
target_include_directories(elfio INTERFACE vendor/elfio)

add_executable(yld main.cpp symbol_resolution.h symbol_resolution.cpp utils.h Relocation.cpp Relocation.h Symbol.cpp Symbol.h ObjectFile.cpp ObjectFile.h)
target_link_libraries(yld PRIVATE elfio)