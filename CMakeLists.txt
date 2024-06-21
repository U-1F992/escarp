cmake_minimum_required(VERSION 3.10)

project(escarp VERSION 0.1.0)
set(CMAKE_C_STANDARD 90)

include_directories(include)
file(GLOB_RECURSE SRC_FILES src/*.c)
add_library(escarp STATIC ${SRC_FILES})

add_executable(test test/test.c)
target_link_libraries(test escarp)
