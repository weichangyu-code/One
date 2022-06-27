cmake_minimum_required(VERSION 3.0.0)
project(engine VERSION 0.1.0)

aux_source_directory(./ ENGINE_SRC_LIST)

add_library(engine STATIC ${ENGINE_SRC_LIST})

