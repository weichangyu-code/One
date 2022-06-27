cmake_minimum_required(VERSION 3.0.0)
project(framework VERSION 0.1.0)

aux_source_directory(./ FRAMEWORK_SRC_LIST)

add_library(framework STATIC ${FRAMEWORK_SRC_LIST})

