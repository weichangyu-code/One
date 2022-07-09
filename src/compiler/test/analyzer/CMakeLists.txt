
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(RuleGenerator RuleGenerator.cpp)
target_link_libraries(RuleGenerator compiler common)