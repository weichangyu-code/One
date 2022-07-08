cmake_minimum_required(VERSION 3.0.0)
project(OneSrc VERSION 0.1.0)

add_definitions(-std=c++11)

add_subdirectory(./compiler)
add_subdirectory(./common)
add_subdirectory(./coroutine)
add_subdirectory(./framework)
