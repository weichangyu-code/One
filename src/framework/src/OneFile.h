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
        File(String* path, OneInt openFlag, OneInt mode = 0777);
        ~File();

        OneInt open(String* path, OneInt openFlag, OneInt mode = 0777);
        void close();
        OneBool isOpen();

        //缓冲读写
        OneInt write(Buffer* buffer);
        OneInt read(Buffer* buffer);

        //字符串处理
        OneInt write(String* str);

    protected:
        OneCoroutine::CoFile* _file;
    };
} // namespace One

