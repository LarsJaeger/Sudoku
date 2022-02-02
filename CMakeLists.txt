cmake_minimum_required(VERSION 3.21)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled1 main.c)
add_library(stack stack/stack.c)
target_link_libraries(untitled1 m)
target_link_libraries(untitled1 stack)