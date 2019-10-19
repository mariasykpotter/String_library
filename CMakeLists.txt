cmake_minimum_required(VERSION 3.14)
project(My_String)

set(CMAKE_C_STANDARD 99)
set(SRCS_FILES library/src/my_str_t.c library/include/my_str_t.h)

add_library(MyString SHARED ${SRCS_FILES})

add_executable(Program main.c)



target_link_libraries(Program MyString)
target_include_directories(MyString PUBLIC library/include)
target_include_directories(Program PUBLIC library/include)