#pragma once
#include "OneObject.h"
#include "OneString.h"

namespace One
{
    class Buffer : public Object
    {
    public:
        Buffer(int capacity = 4*1024);
        ~Buffer();

    public:
        char* getBuf()
        {
            return _buf;
        }
        unsigned int getCapacity()
        {
            return _capacity;
        }

        char* getLeftBuf()
        {
            return _buf + _writePos;
        }
        unsigned int getLeftCapacity()
        {
            return _capacity - _writePos;
        }
        void addWritePos(unsigned int len)
        {
            _writePos += len;
            assert(_writePos <= _capacity);
        }

        char* getData()
        {
            return _buf + _readPos;
        }
        unsigned int getDataLength()
        {
            return _writePos - _readPos;
        }
        void addReadPos(unsigned int len)
        {
            _readPos += len;
            assert(_readPos <= _writePos);
        }

        void clear();
        void put(String* str);

        void put(const char* data, unsigned int len);

    protected:
        void resizeCapacity(unsigned int capacity);

    protected:
        unsigned int _readPos = 0;
        unsigned int _writePos = 0;
        unsigned int _capacity = 0;
        char* _buf = nullptr;
    };
}
