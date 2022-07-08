cmake_minimum_required(VERSION 3.0.0)
project(coctx VERSION 0.1.0)
enable_language(C ASM)

if(WIN32)
set(COROUTINE_SRC_LIST coctx.cpp coctx_swap_rcxrdx.s)
else()
set(COROUTINE_SRC_LIST coctx.cpp coctx_swap_rsirdi.s)
endif()
# 采用兼容性的方案，需要去掉优化
# set(CMAKE_BUILD_TYPE DEBUG)
# set(CMAKE_CXX_FLAGS_DEBUG "-O0")
# set(CMAKE_C_FLAGS_DEBUG "-O0")
# message(STATUS "CMAKE FLAGS ${CMAKE_CXX_FLAGS_DEBUG}")

add_library(coctx STATIC ${COROUTINE_SRC_LIST})