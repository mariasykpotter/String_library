cmake_minimum_required(VERSION 3.14)
project(My_String C)

set(CMAKE_C_STANDARD 99)

add_library(MyString STATIC my_str_t.c my_str_t.h)

add_executable(Program main.c)
target_link_libraries(Program MyString)