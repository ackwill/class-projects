cmake_minimum_required(VERSION 3.3)
project(Euler_Problems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp 12.cpp)
add_executable(Euler_Problems ${SOURCE_FILES})