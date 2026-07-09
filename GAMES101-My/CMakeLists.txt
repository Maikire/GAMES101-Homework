cmake_minimum_required(VERSION 3.10)

project(GAMES101-My)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigen 路径
set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Plugins/Eigen")

# OpenCV
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Plugins/OpenCV/x64/vc16/lib")
find_package(OpenCV REQUIRED)

# 子目录
add_subdirectory(Assignment0)
add_subdirectory(Assignment1)
add_subdirectory(Assignment2)
add_subdirectory(Assignment3)
add_subdirectory(Assignment4)
add_subdirectory(Assignment5)
add_subdirectory(Assignment6)
add_subdirectory(Assignment7)
add_subdirectory(Assignment8)
