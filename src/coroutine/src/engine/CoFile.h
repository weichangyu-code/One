#pragma once

#include "../file/iocp/File.h"
#include "../file/posix/File.h"
#include "Common.h"
#include "CoMutex.h"

namespace OneCoroutine
{
    class CoFile
    {
    public:
        CoFile();
        ~CoFile();

        int  open(const char* path, int openFlag = 0, int mode = 0777);     //mode是8进制的
        void close();
        int  write(const char* data, unsigned int len);
        int  read(char* data, unsigned int len);

        bool isOpen();

    protected:
        CoMutex _mtx;
        File _file;
    };
} // namespace OneCoroutine

