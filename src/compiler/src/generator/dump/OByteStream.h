#pragma once
#include "Common.h"
#include <string>

class OByteStream
{
public:
    OByteStream();

    OByteStream& operator << (const string& str);
    OByteStream& operator << (int i);

    const char*  getData()
    {
        return _buf;
    }
    unsigned int getLength()
    {
        return _writePos;
    }

protected:
    void resizeCapacity(unsigned int capacity);
    void put(const void* data, unsigned int len);

protected:
    char* _buf = nullptr;
    unsigned int _writePos = 0;
    unsigned int _capacity = 0;
};