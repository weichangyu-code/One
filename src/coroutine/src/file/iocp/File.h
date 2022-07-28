#pragma once

#ifdef _WIN32

#include <functional>
#include "List.h"
#include "../../socket/iocp/OperateOverlapped.h"
#include <mutex>
using namespace std;
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
        int  open(const char* path, int openFlag);
        void close();
        bool isOpen();

        void write(const char* data, unsigned int len, OperateOverlapped** ooOut, const OOCompleteCB& cb);
        void read(char* data, unsigned int len, OperateOverlapped** ooOut, const OOCompleteCB& cb);

    protected:
        Iocp* iocp = nullptr;
        HANDLE hFile = INVALID_HANDLE_VALUE;
    };
}

#endif