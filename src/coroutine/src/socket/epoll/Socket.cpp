
#ifndef _WIN32

#include "Socket.h"
#include "../SocketApi.h"
#include "Epoll.h"
#include <string.h>

namespace OneCoroutine
{
    void Socket::onEvent(bool read, bool write, bool exception)
    {
        listener->onEvent(read, write, exception);
    }
        
    Socket::Socket(Epoll* epoll, SocketListener* listener)
    {
        this->epoll = epoll;
        this->listener = listener;
    }
        
    Socket::~Socket()
    {
        close();
    }
        
    int Socket::listen(const char* localAddr, int port, bool reuseaddr, int backlog)
    {
        close();

        sockFd = createSocket(true);
        setSocketAsync(sockFd);
        if (reuseaddr)
        {
            setSocketReuseaddr(sockFd, true);
        }

        sockaddr_in saddr;
        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = INADDR_ANY;
        if (::bind(sockFd, (const struct sockaddr*)&saddr, sizeof(saddr)) != 0)
        {
            close();
            return ERR_SOCKET_BIND_FAILED;
        }

        if (::listen(sockFd, backlog) != 0)
        {
            close();
            return ERR_SOCKET_BIND_FAILED;
        }

        epoll->registerEvent(this, true);

        return ERR_SUCCESS;
    }
        
    int Socket::connect(const char* addr, int port)
    {
        close();
    
        sockFd = createSocket(true);
        setSocketAsync(sockFd);

        sockaddr_in saddr;
        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = inet_addr(addr);
        int ret = ::connect(sockFd, (sockaddr*)&saddr, sizeof(saddr));
        if (ret == -1)
        {
            if (errno != EINPROGRESS)
            {
                close();
                return ERR_ERROR;
            }
        }

        epoll->registerEvent(this, false);

        return (ret == 0) ? ERR_SUCCESS : ERR_SOCKET_INPROGRESS;
    }
        
    int Socket::accept(Socket* listenSocket)
    {
        close();

        sockaddr_in saddr;
        memset(&saddr, 0, sizeof(saddr));
        socklen_t saddr_len = sizeof(saddr);
        sockFd = ::accept(listenSocket->sockFd, (sockaddr*)&saddr, &saddr_len);
        if (sockFd == -1)
        {
            return (errno == EAGAIN) ? ERR_SOCKET_AGAIN : ERR_ERROR;
        }
        setSocketAsync(sockFd);

        epoll->registerEvent(this, false);

        return ERR_SUCCESS;
    }
        
    void Socket::close()
    {
        if (sockFd != -1)
        {
            epoll->unregisterEvent(this);

            ::close(sockFd);
            sockFd = -1;
        }
    }       
    
    int Socket::send(const char* data, unsigned int len)
    {
        if (sockFd == -1)
        {
            return ERR_ERROR;
        }
        int ret = ::send(sockFd, data, len, 0);
        if (ret == -1)
        {
            return (errno == EAGAIN) ? ERR_SOCKET_AGAIN : ERR_ERROR;
        }
        return ret;
    }
        
    int Socket::recv(char* data, unsigned int len)
    {
        if (sockFd == -1)
        {
            return ERR_ERROR;
        }
        int ret = ::recv(sockFd, data, len, 0);
        if (ret == -1)
        {
            return (errno == EAGAIN) ? ERR_SOCKET_AGAIN : ERR_ERROR;
        }
        else if (ret == 0)
        {
            //表示断开
            return ERR_ERROR;
        }
        return ret;
    }
        
    void Socket::setSendBuf(unsigned int sendBuf)
    {
        if (sockFd == -1)
        {
            return;
        }

        setSocketSendBuf(sockFd, sendBuf);
    }
        
    void Socket::setRecvBuf(unsigned int recvBuf)
    {
        if (sockFd == -1)
        {
            return;
        }

        setSocketRecvBuf(sockFd, recvBuf);
    }

} // namespace One

#endif