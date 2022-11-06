cmake_minimum_required(VERSION 3.0.0)

add_executable(FolderUtilsTest FolderUtilsTest.cpp)
target_link_libraries(FolderUtilsTest common)

add_executable(SystemUtilsTest SystemUtilsTest.cpp)
target_link_libraries(SystemUtilsTest common)