#pragma once

#ifdef _WIN32

#include "../Network.h"
#include <functional>
#include "List.h"
using namespace OneCommon;

namespace OneCoroutine
{
    class Socket;
    class File;
    class OperateOverlapped;

    typedef std::function<void(OperateOverlapped* oo)> SocketCompleteCB;

    class OperateOverlapped
    {
    public:
        OVERLAPPED ol;

        enum
        {
            TYPE_NONE = 0,

            TYPE_NOTIFY,

            TYPE_SOCKET_ACCEPT,
            TYPE_SOCKET_CONNECT,
            TYPE_SOCKET_SEND,
            TYPE_SOCKET_RECV,

            TYPE_FILE_OPEN,
            TYPE_FILE_WRITE,
            TYPE_FILE_READ,
        };
        int type = 0;

        Socket* socket = nullptr;
        File* file = nullptr;
        int trans = 0;
        int error = 0;
        char* buffer = nullptr;

        SocketCompleteCB cb;

        ListNode poolNode;
    };
}

#endif
