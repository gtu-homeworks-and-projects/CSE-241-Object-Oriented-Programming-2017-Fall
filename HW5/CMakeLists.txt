cmake_minimum_required(VERSION 3.8)
project(HW5)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Cell.cpp Cell.h ConnectFourAbstract.cpp ConnectFourAbstract.h ConnectFourDiag.cpp ConnectFourDiag.h ConnectFourPlus.cpp ConnectFourPlus.h ConnectFourPlusUndo.cpp ConnectFourPlusUndo.h)
add_executable(HW5 ${SOURCE_FILES})