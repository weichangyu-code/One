
add_subdirectory(./analyzer)
add_subdirectory(./generator)
if (EXISTS ${CMAKE_SOURCE_DIR}/compiler/test/onetest/CMakeLists.txt)
add_subdirectory(./onetest)
endif()
