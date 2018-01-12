cmake_minimum_required(VERSION 3.9)
project(CarBrandDetector)
project(CarBrandDetectorTest)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE( OpenCV REQUIRED )

include_directories(headers)

file(GLOB SOURCES
        src/*.cpp
        )
file(GLOB TEST_SOURCES
        test/*cpp)

add_executable(CarBrandDetector main/main.cpp ${SOURCES})
target_link_libraries( CarBrandDetector ${OpenCV_LIBS} )

add_executable(CarBrandDetectorTest ${TEST_SOURCES} ${SOURCES})
target_link_libraries( CarBrandDetectorTest ${OpenCV_LIBS} )