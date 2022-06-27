#ifdef _WIN32

#include "Socket.h"
#include "../SocketApi.h"
#include "Iocp.h"
#include <string.h>

namespace OneCoroutine
{
    void Socket::onEvent(OperateOverlapped* oo)
    {
        if (oo->type == OperateOverlapped::TYPE_ACCEPT)
        {
            if (oo->error != 0)
            {
                close();
            }
            oo->cb(oo);
            delete oo->buffer;
        }
        else if (oo->type == OperateOverlapped::TYPE_CONNECT)
        {
            if (oo->error != 0)
            {
                close();
            }
            oo->cb(oo);
        }
        else if (oo->type == OperateOverlapped::TYPE_RECV)
        {
            if (oo->error == 0 && oo->trans == 0)
            {
                //trans=0代表断线
                oo->error = SOCKET_ERR_ERROR;
            }
            oo->cb(oo);
        }
        else
        {
            oo->cb(oo);
        }
        iocp->freeToPool(oo);
    }
        
    Socket::Socket(Iocp* iocp)
    {
        this->iocp = iocp;
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
            return SOCKET_ERR_BIND_FAILED;
        }

        if (::listen(sockFd, backlog) != 0)
        {
            close();
            return SOCKET_ERR_BIND_FAILED;
        }

        iocp->registerEvent(this);

        return SOCKET_ERR_SUCCESS;
    }
        
    LPFN_CONNECTEX Socket::getConnectExFunc()
    {
        LPFN_CONNECTEX funcConnectEx = nullptr;
        DWORD dwBytes = 0;
        GUID guidConnectEx = WSAID_CONNECTEX;
        if(SOCKET_ERROR == WSAIoctl(sockFd, SIO_GET_EXTENSION_FUNCTION_POINTER,
            &guidConnectEx, sizeof(guidConnectEx), &funcConnectEx, sizeof(funcConnectEx), &dwBytes, NULL, NULL))
        {
            return nullptr;
        }
        return funcConnectEx;
    }
        
    void Socket::connect(const char* addr, int port, OperateOverlapped** ooOut, const SocketCompleteCB& cb)
    {
        close();
    
        sockFd = createSocket(true);
        setSocketAsync(sockFd);
        
        iocp->registerEvent(this);
        
        OperateOverlapped* oo = iocp->mallocFromPool();
        oo->type = OperateOverlapped::TYPE_CONNECT;
        oo->socket = this;
        oo->cb = cb;
        *ooOut = oo;

        sockaddr_in laddr;
        memset(&laddr, 0, sizeof(laddr));
        laddr.sin_family = AF_INET;
        laddr.sin_addr.s_addr = INADDR_ANY;
        laddr.sin_port = 0;
        if (::bind(sockFd, (sockaddr*)&laddr, sizeof(laddr)) < 0)
        {
            oo->error = SOCKET_ERR_BIND_FAILED;
            onEvent(oo);
            return;
        }

        sockaddr_in saddr;
        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = inet_addr(addr);

        LPFN_CONNECTEX ConnectEx = getConnectExFunc();
        bool ret = ConnectEx(sockFd, (sockaddr*)&saddr, sizeof(saddr), nullptr, 0, (LPDWORD)&oo->trans, &oo->ol);
        if (ret == false)
        {
            int err = GetLastError();
            if (err != WSA_IO_PENDING)
            {
                oo->error = SOCKET_ERR_ERROR;
                onEvent(oo);
                return;
            }
        }
        else
        {
            onEvent(oo);
        }
    }
        
    void Socket::accept(Socket* listenSocket, OperateOverlapped** ooOut, const SocketCompleteCB& cb)
    {
        close();

        sockFd = createSocket(true);
        setSocketAsync(sockFd);
        
        iocp->registerEvent(this);

        OperateOverlapped* oo = iocp->mallocFromPool();
        oo->type = OperateOverlapped::TYPE_ACCEPT;
        oo->socket = this;
        oo->cb = cb;
        oo->buffer = new char[128];
        *ooOut = oo;

        socklen_t saddr_len = sizeof(sockaddr_in) + 16;
        bool ret = ::AcceptEx(listenSocket->sockFd, sockFd, oo->buffer, 0, saddr_len, saddr_len
            , (LPDWORD)&oo->trans, &oo->ol);
        if (ret == false)
        {
            int err = GetLastError();
            if (err != WSA_IO_PENDING)
            {
                oo->error = SOCKET_ERR_ERROR;
                onEvent(oo);
                return;
            }
        }
        else
        {
            onEvent(oo);
        }
    }
        
    void Socket::close()
    {
        if (sockFd != -1)
        {
            iocp->unregisterEvent(this);

            closesocket(sockFd);
            sockFd = -1;
        }
    }       
    
    void Socket::send(const char* data, unsigned int len, OperateOverlapped** ooOut, const SocketCompleteCB& cb)
    {
        if (sockFd == -1)
        {
            return;
        }

        WSABUF wsaBuf;
        wsaBuf.buf = (char*)data;
        wsaBuf.len = len;

        OperateOverlapped* oo = iocp->mallocFromPool();
        oo->type = OperateOverlapped::TYPE_SEND;
        oo->socket = this;
        oo->cb = cb;
        *ooOut = oo;

        int ret = ::WSASend(sockFd, &wsaBuf, 1, (LPDWORD)&oo->trans, 0, &oo->ol, NULL);
        if (ret < 0)
        {
            int err = GetLastError();
            if (err != WSA_IO_PENDING)
            {
                oo->error = SOCKET_ERR_ERROR;
                onEvent(oo);
                return;
            }
        }
        else
        {
            //成功也会通过GetQueuedCompletionStatus回调
            //onEvent(oo);
        }
    }
        
    void Socket::recv(char* data, unsigned int len, OperateOverlapped** ooOut, const SocketCompleteCB& cb)
    {
        if (sockFd == -1)
        {
            return;
        }

        WSABUF wsaBuf;
        wsaBuf.buf = data;
        wsaBuf.len = len;

        OperateOverlapped* oo = iocp->mallocFromPool();
        oo->type = OperateOverlapped::TYPE_RECV;
        oo->socket = this;
        oo->cb = cb;
        *ooOut = oo;

        DWORD flags = 0;
        int ret = ::WSARecv(sockFd, &wsaBuf, 1, (LPDWORD)&oo->trans, &flags, &oo->ol, nullptr);
        if (ret < 0)
        {
            int err = GetLastError();
            if (err != WSA_IO_PENDING)
            {
                oo->error = SOCKET_ERR_ERROR;
                onEvent(oo);
                return;
            }
        }
        else
        {
            //成功也会通过GetQueuedCompletionStatus回调
            //onEvent(oo);
        }
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
        
    Engine* Socket::getEngine()
    {
        return iocp->getEngine();
    }
        
    void Socket::cancelIo(OperateOverlapped* oo)
    {
        iocp->cancelIo(this, oo);
    }



} // namespace One

#endif