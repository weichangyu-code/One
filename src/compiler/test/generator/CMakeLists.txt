
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/compiler/test/generator)

add_executable(CppGeneratorTest CppGeneratorTest.cpp)
target_link_libraries(CppGeneratorTest compiler common)
