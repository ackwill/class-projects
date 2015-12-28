cmake_minimum_required(VERSION 3.3)
project(FaceRecognizer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp FaceRecognizer.cpp FaceRecognizer.h LBPHTrainer.cpp LBPHTrainer.hpp)
add_executable(FaceRecognizer ${SOURCE_FILES})

target_link_libraries( FaceRecognizer ${OpenCV_LIBS} )