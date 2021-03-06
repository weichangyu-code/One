﻿enable_language(C ASM)

#
if(WIN32)
set(SEARCH_LIST ./coctx/coctx.cpp ./coctx/coctx_swap_rcxrdx.s)
else()
set(SEARCH_LIST ./coctx/coctx.cpp ./coctx/coctx_swap_rsirdi.s)
endif()
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})

#
aux_source_directory(./engine SEARCH_LIST)
set(SRC_LIST ${SRC_LIST} ${SEARCH_LIST})

#
aux_source_directory(./socket/epoll EPOLL_SRC_LIST)
aux_source_directory(./socket/iocp IOCP_SRC_LIST)
aux_source_directory(./socket/ SOCKET_SRC_LIST)
set(SRC_LIST ${SRC_LIST} ${EPOLL_SRC_LIST} ${IOCP_SRC_LIST} ${SOCKET_SRC_LIST})

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/lib)
add_library(coroutine STATIC ${SRC_LIST})


add_custom_command(TARGET coroutine POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/coctx
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/engine
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/socket
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/socket/epoll
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/socket/iocp
    )


file(GLOB COROUTINE_INCLUDE_LIST ./coctx/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/coctx/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./engine/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/engine/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./socket/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/socket/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./socket/epoll/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/socket/epoll/
        )
endforeach()

file(GLOB COROUTINE_INCLUDE_LIST ./socket/iocp/*.h)
foreach(INCLUDE_FILE IN LISTS COROUTINE_INCLUDE_LIST)
    add_custom_command(TARGET coroutine POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/coroutine/socket/iocp/
        )
endforeach()
