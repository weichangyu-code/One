#pragma once

#ifdef _WIN32

#include <functional>
#include "List.h"
#include "../../socket/iocp/OperateOverlapped.h"
using namespace OneCommon;

namespace OneCoroutine
{
    class Iocp;
    class File
    {
        friend class Iocp;
    public:
        File(Iocp* iocp);
        ~File();

    public:
        bool open(const string& path, const string& mode, OperateOverlapped** ooOut, const SocketCompleteCB& cb);

    protected:
        Iocp* iocp = nullptr;
        HANDLE hFile = INVALID_HANDLE_VALUE;
    };
}

#endif