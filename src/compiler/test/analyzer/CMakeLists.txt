
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/compiler/test/analyzer)

add_executable(RuleGenerator RuleGenerator.cpp)
target_link_libraries(RuleGenerator compiler common)