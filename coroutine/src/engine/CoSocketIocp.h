#pragma once
#ifdef _WIN32

#include "../socket/iocp/Socket.h"
#include <climits>

namespace OneCoroutine
{
    class Engine;
    class CoSocket
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
        void cancelIo(OperateOverlapped* oo);
        
    protected:
        Socket socket;
    };
} // namespace One

#endif