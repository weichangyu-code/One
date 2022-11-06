
if(MSVC)
#打开异常
add_definitions(/EHa /wd4819 /wd4996)
else()
#寄存器r15用于存储engine
add_definitions(-std=c++11 -ffixed-r15 -fnon-call-exceptions)
endif()

add_subdirectory(./common)
add_subdirectory(./compiler)
add_subdirectory(./coroutine)
add_subdirectory(./framework)
