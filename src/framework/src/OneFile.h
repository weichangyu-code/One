#pragma once
#include "OneObject.h"
#include "OneString.h"
#include "OneBuffer.h"

namespace OneCoroutine
{
    class CoFile;
}

namespace One
{
    class File : public Object
    {
    public:
        File();
        File(String* path, int openFlag, int mode = 0777);
        ~File();

        int  open(String* path, int openFlag, int mode = 0777);
        void close();
        bool isOpen();

        //缓冲读写
        int  write(Buffer* buffer);
        int  read(Buffer* buffer);

        //字符串处理
        int  write(String* str);

    protected:
        OneCoroutine::CoFile* _file;
    };
} // namespace One

