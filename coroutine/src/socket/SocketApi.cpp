#include "SocketApi.h"

namespace OneCoroutine
{
    SOCKET createSocket(bool tcpOrUdp)
    {
        if (tcpOrUdp)
        {
            return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        }
        else
        {
            return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        }
    }

    void setSocketAsync(SOCKET sock)
    {
    #if defined(_WIN32)
        unsigned long l = 1;
        ioctlsocket(sock, FIONBIO, &l);
    #else
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    #endif
    }

    void setSocketOption(SOCKET sock, int level, int type, void* value, unsigned int valueLen)
    {
        ::setsockopt(sock, level, type, (char*)value, valueLen);
    }

    void getSocketOption(SOCKET sock, int level, int type, void* value, unsigned int valueLen)
    {
        ::getsockopt(sock, level, type, (char*)value, (socklen_t*)&valueLen);
    }
        
    void setSocketLinger(SOCKET sock, int value)
    {
        struct linger data;
        data.l_onoff = (value > 0) ? 1 : 0;
        data.l_linger = value;
        setSocketOption(sock, SOL_SOCKET, SO_LINGER, &data, sizeof(data));
    }
        
    void setSocketNodelay(SOCKET sock, bool value)
    {
        int optval = value ? 1 : 0;
        setSocketOption(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&optval, sizeof(optval));
    }
        
    void setSocketSendBuf(SOCKET sock, unsigned int size)
    {
        setSocketOption(sock, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    }

    void setSocketRecvBuf(SOCKET sock, unsigned int size)
    {
        setSocketOption(sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    }
    
    unsigned int getSocketSendBuf(SOCKET sock)
    {
        unsigned int size = 0;
        getSocketOption(sock, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
        return size;
    }
    
    unsigned int getSocketRecvBuf(SOCKET sock)
    {
        unsigned int size = 0;
        getSocketOption(sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
        return size;
    }

    void setSocketReuseaddr(SOCKET sock, bool value)
    {
        int data = value;
        setSocketOption(sock, SOL_SOCKET, SO_REUSEADDR, &data, sizeof(data));
    }
    
    void setSocketSendTimeout(SOCKET sock, unsigned int timeout)
    {
        setSocketOption(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }
    
    void setSocketRecvTimeout(SOCKET sock, unsigned int timeout)
    {
        setSocketOption(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
}
	