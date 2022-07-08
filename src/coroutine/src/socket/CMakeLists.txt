
aux_source_directory(./epoll EPOLL_SRC_LIST)
aux_source_directory(./iocp IOCP_SRC_LIST)
aux_source_directory(./ SOCKET_SRC_LIST)

add_library(socket STATIC ${SOCKET_SRC_LIST} ${EPOLL_SRC_LIST} ${IOCP_SRC_LIST})

