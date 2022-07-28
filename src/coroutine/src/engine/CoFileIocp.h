#pragma once

#ifdef _WIN32

#include "../file/iocp/File.h"
#include "Common.h"
#include "CoMutex.h"

namespace OneCoroutine
{
    class CoFile
    {
    public:
        CoFile();
        ~CoFile();

        int  open(const char* path, int openFlag = 0);
        void close();
        int  write(const char* data, unsigned int len);
        int  read(char* data, unsigned int len);

    protected:
        CoMutex _mtx;
        File _file;
    };
} // namespace OneCoroutine

#endif
