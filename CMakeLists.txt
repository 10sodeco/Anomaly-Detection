cmake_minimum_required(VERSION 2.8)
PROJECT(TestCV)
find_package(OpenCV REQUIRED)
add_executable( TestCV TestCV.cpp)
target_link_libraries( TestCV ${OpenCV_LIBS})
