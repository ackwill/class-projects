cmake_minimum_required(VERSION 2.8)
project( LBPHFaceRecognizer_Trainer )
find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#add_executable( LBPHFaceRecognizer_Trainer LBPHFaceRecognizer_Trainer.cpp )

set(SOURCE_FILES recog.cpp)
add_executable(LBPHFaceRecognizer_Trainer ${SOURCE_FILES})
target_link_libraries( LBPHFaceRecognizer_Trainer ${OpenCV_LIBS} )

