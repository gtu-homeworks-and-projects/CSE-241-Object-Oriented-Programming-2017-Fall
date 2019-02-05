cmake_minimum_required(VERSION 3.8)
project(HW3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ConnectFour.cpp ConnectFour.h)
add_executable(HW3 ${SOURCE_FILES})