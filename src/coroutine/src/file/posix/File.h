#pragma once

#ifndef _WIN32

namespace OneCoroutine
{
    class File
    {
    public:
        File();
        ~File();

    public:
        int  open(const char* path, int openFlag, int mode);
        void close();
        bool isOpen();

        int  write(const char* data, unsigned int len);
        int  read(char* data, unsigned int len);

    protected:
        int _fd = -1;
    };
}

#endif