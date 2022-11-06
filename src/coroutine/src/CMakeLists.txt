
if(MSVC)
enable_language(ASM_MASM)
else()
enable_language(ASM)
endif()

#
if(MSVC)
set(SEARCH_LIST ./coctx/coctx.cpp ./coctx/coctx_swap_x86_64_win_masm.asm)
elseif(WIN32)
set(SEARCH_LIST ./coctx/coctx.cpp ./coctx/coctx_swap_x86_64_win_gas.s)
else()
set(SEARCH_LIST ./coctx/coctx.cpp ./coctx/coctx_swap_x86_64_linux_gas.s)
endif()
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})

#
aux_source_directory(./thread SEARCH_LIST)
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})

#
aux_source_directory(./file/iocp SEARCH_LIST)
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})
aux_source_directory(./file/posix SEARCH_LIST)
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})

#
aux_source_directory(./engine SEARCH_LIST)
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})

#
aux_source_directory(./socket/epoll EPOLL_SRC_LIST)
aux_source_directory(./socket/iocp IOCP_SRC_LIST)
aux_source_directory(./socket/ SOCKET_SRC_LIST)
set(SRC_LIST ${SRC_LIST} ${EPOLL_SRC_LIST} ${IOCP_SRC_LIST} ${SOCKET_SRC_LIST})

if(MSVC)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/lib>)
else()
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/lib)
endif()
add_library(coroutine STATIC ${SRC_LIST})

add_custom_command(TARGET coroutine POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/coctx
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/thread
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/engine
    )


file(GLOB COROUTINE_INCLUDE_LIST ./coctx/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/coctx/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./thread/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/thread/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./thread/*.inl)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/thread/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./engine/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/engine/
        )
endforeach()
