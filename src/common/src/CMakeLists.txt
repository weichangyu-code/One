
aux_source_directory(./ SRC_LIST)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/lib)
add_library(common STATIC ${SRC_LIST})

add_custom_command(TARGET common POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/common
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/common
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#拷贝头文件
file(GLOB COMMON_INCLUDE_LIST *.h)
foreach(INCLUDE_FILE IN LISTS COMMON_INCLUDE_LIST)
    add_custom_command(TARGET common POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/common/
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

