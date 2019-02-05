cmake_minimum_required(VERSION 3.8)
project(HW4)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ConnectFour.cpp ConnectFour.h main.cpp)
add_executable(HW4 ${SOURCE_FILES})