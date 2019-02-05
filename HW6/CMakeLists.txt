cmake_minimum_required(VERSION 3.9)
project(HW6)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp GTUSetBase.cpp GTUSetBase.h GTUSet.cpp GTUSet.h GTUMap.cpp GTUMap.h)
add_executable(HW6 ${SOURCE_FILES})