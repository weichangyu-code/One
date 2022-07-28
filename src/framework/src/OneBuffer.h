#pragma once
#include "OneObject.h"

namespace One
{
    class Buffer : public Object
    {
    public:
        Buffer(int length);
        ~Buffer();

    public:
        unsigned char* getBuf()
        {
            return _buf;
        }
        unsigned int getLength()
        {
            return _length;
        }

    public:
        unsigned int _readPos = 0;
        unsigned int _writePos = 0;
    protected:
        unsigned int _length = 0;
        unsigned char* _buf = nullptr;
    };
}
