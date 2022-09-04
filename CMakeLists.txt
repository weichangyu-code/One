cmake_minimum_required(VERSION 3.0.0)
project(One VERSION 0.1.0)

add_subdirectory(./src)
add_subdirectory(./depend)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/one/HttpServer/build/CMakeLists.txt")
add_subdirectory(./one/HttpServer/build)
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/one/HttpClient/build/CMakeLists.txt")
add_subdirectory(./one/HttpClient/build)
endif()

