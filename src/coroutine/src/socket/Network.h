#pragma once
#include "../common/Error.h"

#ifdef _WIN32

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#else

#define SOCKET  int

#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>		/* TCP_NODELAY */
#include <arpa/inet.h>	/* inet(3) functions */
#include <sys/types.h>
#include <fcntl.h>		/* for nonblocking */
#include <errno.h>
#include <sys/epoll.h>
#include <sys/uio.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>

#endif
