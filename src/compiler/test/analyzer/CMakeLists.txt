cmake_minimum_required(VERSION 3.0.0)
project(AnalyzerTest VERSION 0.1.0)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/compiler/test/analyzer)

add_executable(RuleGenerator RuleGenerator.cpp)
target_link_libraries(RuleGenerator compiler common)