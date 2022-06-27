cmake_minimum_required(VERSION 3.0.0)
project(common VERSION 0.1.0)

aux_source_directory(./ SRC_LIST)

add_library(common STATIC ${SRC_LIST})



