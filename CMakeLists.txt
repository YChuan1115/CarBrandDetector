cmake_minimum_required(VERSION 3.9)
project(CarBrandDetector)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE( OpenCV REQUIRED )

file(GLOB SOURCES
        headers/*.h
        src/*.cpp
        )

add_executable(CarBrandDetector ${SOURCES} headers/sobel.hpp src/sobel.cpp headers/shapeRecognition.hpp src/shapeRecognition.cpp)
target_link_libraries( CarBrandDetector ${OpenCV_LIBS} )