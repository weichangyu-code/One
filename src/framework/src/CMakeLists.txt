
aux_source_directory(./ FRAMEWORK_SRC_LIST)

if(MSVC)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/lib>)
else()
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/lib)
endif()
add_library(framework STATIC ${FRAMEWORK_SRC_LIST})

add_custom_command(TARGET framework POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/one
    COMMAND ${CMAKE_COMMAND} -E copy_directory ../one ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/one
    COMMAND ${CMAKE_COMMAND} -E copy ../one.json ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/framework
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/framework
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#拷贝头文件
file(GLOB FRAMEWORK_INCLUDE_LIST *.h)
foreach(INCLUDE_FILE IN LISTS FRAMEWORK_INCLUDE_LIST)
    add_custom_command(TARGET framework POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/framework/)
endforeach()

#拷贝头文件
file(GLOB FRAMEWORK_INCLUDE_LIST *.inl)
foreach(INCLUDE_FILE IN LISTS FRAMEWORK_INCLUDE_LIST)
    add_custom_command(TARGET framework POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${INCLUDE_FILE} ${CMAKE_SOURCE_DIR}/dist/depend/frameworkV0.1/include/framework/)
endforeach()

