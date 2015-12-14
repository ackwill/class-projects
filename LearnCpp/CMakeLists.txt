cmake_minimum_required(VERSION 3.3)
project(TicTacToe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp TicTacToe.cpp TicTacToe.h)
add_executable(TicTacToe ${SOURCE_FILES})