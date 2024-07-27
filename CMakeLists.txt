cmake_minimum_required(VERSION 3.10)

project(escarp VERSION 0.1.0)
set(CMAKE_C_STANDARD 90)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_library(escarp STATIC ${SRC_FILES})
target_include_directories(escarp PUBLIC include)

enable_testing()

add_executable(escarp_test_any test/test_any.c)
target_link_libraries(escarp_test_any escarp)
add_test(NAME escarp_test_any COMMAND escarp_test_any)

add_executable(escarp_test_value test/test_value.c)
target_link_libraries(escarp_test_value escarp)
add_test(NAME escarp_test_value COMMAND escarp_test_value)

add_executable(escarp_test_sequence test/test_sequence.c)
target_link_libraries(escarp_test_sequence escarp)
add_test(NAME escarp_test_sequence COMMAND escarp_test_sequence)

add_executable(escarp_test_repeat test/test_repeat.c)
target_link_libraries(escarp_test_repeat escarp)
add_test(NAME escarp_test_repeat COMMAND escarp_test_repeat)
