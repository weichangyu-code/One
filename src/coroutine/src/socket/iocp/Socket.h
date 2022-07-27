#pragma once

#ifdef _WIN32

#include "../Network.h"
#include <functional>
#include "List.h"
#include "OperateOverlapped.h"
using namespace OneCommon;

namespace OneCoroutine
{
    class Engine;
    class Iocp;
    class Socket
    {
        friend class Iocp;
    public:
        Socket(Iocp* iocp);
        ~Socket();

        int listen(const char* localAddr, int port, bool reuseaddr, int backlog);
        void connect(const char* addr, int port, OperateOverlapped** ooOut, const SocketCompleteCB& cb);
        void accept(Socket* listenSocket, OperateOverlapped** ooOut, const SocketCompleteCB& cb);
        void close();

        void send(const char* data, unsigned int len, OperateOverlapped** ooOut, const SocketCompleteCB& cb);
        void recv(char* data, unsigned int len, OperateOverlapped** ooOut, const SocketCompleteCB& cb);
        
        void cancelIo(OperateOverlapped* oo);

        void setSendBuf(unsigned int sendBuf);
        void setRecvBuf(unsigned int recvBuf);

        Engine* getEngine();

    protected:
        void onEvent(OperateOverlapped* oo);

        LPFN_CONNECTEX getConnectExFunc();

    protected:
        Iocp* iocp = nullptr;
        int sockFd = -1;
    };
} // namespace One

#endif