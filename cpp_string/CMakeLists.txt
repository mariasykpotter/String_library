cmake_minimum_required(VERSION 3.14)
project(myCppString)

set(CMAKE_CXX_STANDARD 14)
set(SRCS_FILES MyString.cpp MyString.h)



add_library(MyString SHARED ${SRCS_FILES})