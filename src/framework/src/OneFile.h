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
        ~File();

        int  open(String* path, int openFlag);
        void close();
        int  write(Buffer* buffer);
        int  read(Buffer* buffer);

    protected:
        OneCoroutine::CoFile* _file;
    };
} // namespace One

