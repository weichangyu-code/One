
include_directories(../../../common/src ../../src)

add_executable(RuleGenerator RuleGenerator.cpp)
target_link_libraries(RuleGenerator compiler common)