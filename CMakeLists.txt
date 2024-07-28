cmake_minimum_required(VERSION 3.10)

project(escarp VERSION 0.1.0)
set(CMAKE_C_STANDARD 90)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_library(escarp STATIC ${SRC_FILES})
target_include_directories(escarp PUBLIC include)

enable_testing()

add_executable(escarp_test_stream test/test_stream.c)
target_link_libraries(escarp_test_stream escarp)
add_test(NAME escarp_test_stream COMMAND escarp_test_stream)

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

add_executable(escarp_test_choice test/test_choice.c)
target_link_libraries(escarp_test_choice escarp)
add_test(NAME escarp_test_choice COMMAND escarp_test_choice)

add_executable(escarp_test_ignore test/test_ignore.c)
target_link_libraries(escarp_test_ignore escarp)
add_test(NAME escarp_test_ignore COMMAND escarp_test_ignore)

add_executable(escarp_test_complex_examples test/test_complex_examples.c)
target_link_libraries(escarp_test_complex_examples escarp)
add_test(NAME escarp_test_complex_examples COMMAND escarp_test_complex_examples)
