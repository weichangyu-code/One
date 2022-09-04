#include "OneBuffer.h"
#include <string.h>

namespace One
{
    Buffer::Buffer(int capacity)
    {
        _buf = new char[capacity];
        _capacity = capacity;
    }
    Buffer::~Buffer()
    {
        delete[] _buf;
    }
        
    void Buffer::put(String* str)
    {
        put(str->str(), str->length());
    }
        
    void Buffer::put(const char* data, unsigned int len)
    {
        if (_writePos + len > _capacity)
        {
            resizeCapacity(_writePos + len);
        }
        memcpy(_buf + _writePos, data, len);
        _writePos += len;
    }
       
    void Buffer::resizeCapacity(unsigned int capacity)
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
        
    void Buffer::clear()
    {
        _readPos = 0;
        _writePos = 0;
    }
} // namespace One

