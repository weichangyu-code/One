#pragma once

#ifndef _WIN32

#include "../socket/epoll/Socket.h"
#include <climits>
#include "CoEvent.h"

namespace OneCoroutine
{
    class Engine;
    class CoSocket : public SocketListener
    {
    public:
        CoSocket();
        CoSocket(Engine* engine);
        ~CoSocket();

        int listen(const char* localAddr, int port, bool reuseaddr, int backlog);
        int connect(const char* addr, int port, unsigned int timeout);
        int accept(CoSocket* listenSocket, unsigned int timeout = UINT_MAX);
        void close();

        int send(const char* data, unsigned int len, unsigned int timeout = UINT_MAX);
        int recv(char* data, unsigned int len, unsigned int timeout = UINT_MAX);
        
        void setSendBuf(unsigned int sendBuf);
        void setRecvBuf(unsigned int recvBuf);

        Engine* getEngine();

    protected:
        virtual void onEvent(bool read, bool write, bool exception);

    protected:
        Socket socket;

        CoEvent connectEvent;
        CoEvent writeEvent;
        CoEvent readEvent;
        CoEvent exceptEvent;
    };
} // namespace One

#endif