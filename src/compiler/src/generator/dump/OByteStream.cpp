#include "OByteStream.h"
#include <string.h>

OByteStream::OByteStream()
{
    _capacity = 4096;
    _buf = new char[_capacity];
}

void OByteStream::resizeCapacity(unsigned int capacity)
{
    if (_capacity >= capacity)
    {
        return;
    }
    while (_capacity < capacity)
    {
        _capacity *= 2;
    }
    char* buf = new char[_capacity];
    memcpy(buf, _buf, _writePos);
    delete[] _buf;
    _buf = buf;
}
    
OByteStream& OByteStream::operator << (const string& str)
{
    *this << str.length();
    put(str.c_str(), str.length());
    return *this;
}
    
OByteStream& OByteStream::operator << (int i)
{
    put(&i, sizeof(i));
    return *this;
}
    
void OByteStream::put(const void* data, unsigned int len)
{
    resizeCapacity(_writePos + len);
    memcpy(_buf + _writePos, data, len);
    _writePos += len;
}

