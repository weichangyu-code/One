
include_directories(../../include)

aux_source_directory(./ JSON_SRC_LIST)

add_library(json STATIC ${JSON_SRC_LIST})


