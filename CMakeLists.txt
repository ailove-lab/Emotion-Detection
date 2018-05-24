cmake_minimum_required(VERSION 2.8)
project(FaceDetection)

include(/usr/lib/cmake/dlib/dlib.cmake)

set(FaceDerection_VERSION_MAJOR 1)
set(FaceDerection_VERSION_MINOR 0)

SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3")
SET(CMAKE_C_FLAGS_DISTRIBUTION "-O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
link_directories("${CMAKE_BINARY_DIR}/res")

find_package( OpenCV REQUIRED )

add_executable(annotate annotate.cpp )
target_link_libraries(annotate dlib::dlib)

add_executable(train train.cpp )
target_link_libraries(train dlib::dlib)
 
add_executable(emotionRecognition emotionRecognition.cpp )
target_link_libraries(emotionRecognition dlib::dlib)

add_executable(trainMulticlass trainMulticlass.cpp )
target_link_libraries( trainMulticlass dlib::dlib)

add_executable(emotionRecognitionMulticlass emotionRecognitionMulticlass.cpp )
target_link_libraries( emotionRecognitionMulticlass dlib::dlib)

add_executable(objectDetection objectDetection.cpp )
target_link_libraries( objectDetection ${OpenCV_LIBS})

add_executable(train4ClassProbablityOVO train4ClassProbablityOVO.cpp )
target_link_libraries( train4ClassProbablityOVO dlib::dlib)

add_executable(emotionRecognition4ClassProbablityOVO emotionRecognition4ClassProbablityOVO.cpp )
target_link_libraries( emotionRecognition4ClassProbablityOVO dlib::dlib)

add_executable(train4ClassProbablityOVA train4ClassProbablityOVA.cpp )
target_link_libraries( train4ClassProbablityOVA dlib::dlib)

add_executable(emotionRecognition4ClassProbablityOVA emotionRecognition4ClassProbablityOVA.cpp )
target_link_libraries( emotionRecognition4ClassProbablityOVA dlib::dlib)

