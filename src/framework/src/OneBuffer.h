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

    //one
    public:
        void clear();
        void put(String* str);
        Reference<String> getLine();
        Reference<String> getString(OneInt len = -1);
        virtual Reference<String> toString();

    public:
        OneChar* getBuf()
        {
            return _buf;
        }
        unsigned int getCapacity()
        {
            return _capacity;
        }

        OneChar* getLeftBuf()
        {
            return _buf + _writePos;
        }
        OneInt getLeftCapacity()
        {
            return _capacity - _writePos;
        }
        void addWritePos(OneInt len)
        {
            _writePos += len;
            assert(_writePos <= _capacity);
        }

        OneChar* getData()
        {
            return _buf + _readPos;
        }
        OneInt getDataLength()
        {
            return _writePos - _readPos;
        }
        void addReadPos(OneInt len)
        {
            _readPos += len;
            assert(_readPos <= _writePos);
        }

        void put(const OneChar* data, OneInt len);

    protected:
        void resizeCapacity(OneInt capacity);
        int  getLineLength();

    protected:
        OneInt _readPos = 0;
        OneInt _writePos = 0;
        OneInt _capacity = 0;
        OneChar* _buf = nullptr;
    };
}
