cmake_minimum_required(VERSION 3.0.0)

add_executable(CoctxSwapTest CoctxSwapTest.cpp)
target_link_libraries(CoctxSwapTest coroutine common)

add_executable(CoroutineTest CoroutineTest.cpp)
target_link_libraries(CoroutineTest coroutine common)

add_executable(CoSleepTest CoSleepTest.cpp)
target_link_libraries(CoSleepTest coroutine common)

add_executable(CoServerTest CoServerTest.cpp)
target_link_libraries(CoServerTest coroutine common)

add_executable(CoClientTest CoClientTest.cpp)
target_link_libraries(CoClientTest coroutine common)

add_executable(CoFileTest CoFileTest.cpp)
target_link_libraries(CoFileTest coroutine common)

if(WIN32)
  target_link_libraries(CoroutineTest wsock32 ws2_32)
  target_link_libraries(CoSleepTest wsock32 ws2_32)
  target_link_libraries(CoServerTest wsock32 ws2_32)
  target_link_libraries(CoClientTest wsock32 ws2_32)
  target_link_libraries(CoFileTest wsock32 ws2_32)
else()
  find_package(Threads)
  target_link_libraries(CoroutineTest ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(CoSleepTest ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(CoServerTest ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(CoClientTest ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(CoFileTest ${CMAKE_THREAD_LIBS_INIT})
endif()