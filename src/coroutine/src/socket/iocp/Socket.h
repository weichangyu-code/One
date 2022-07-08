#pragma once

#ifdef _WIN32

#include "../Network.h"
#include <functional>
#include "List.h"
using namespace OneCommon;

namespace OneCoroutine
{
    class Socket;
    class OperateOverlapped;

    typedef std::function<void(OperateOverlapped* oo)> SocketCompleteCB;

    class OperateOverlapped
    {
    public:
        OVERLAPPED ol;

        enum
        {
            TYPE_NONE = 0,
            TYPE_ACCEPT,
            TYPE_CONNECT,
            TYPE_SEND,
            TYPE_RECV,
        };
        int type = 0;

        Socket* socket = nullptr;
        int trans = 0;
        int error = 0;
        char* buffer = nullptr;

        SocketCompleteCB cb;

        ListNode poolNode;
    };

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