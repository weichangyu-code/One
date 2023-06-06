#include "OneBuffer.h"
#include <string.h>

namespace One
{
    //Buffer会在堆栈构造
    Buffer::Buffer(int capacity)
    {
        //TODO:后续优化内存
        _buf = new OneChar[capacity];
        _capacity = capacity;
    }
    Buffer::~Buffer()
    {
        delete[] _buf;
    }
        
    void Buffer::clear()
    {
        _readPos = 0;
        _writePos = 0;
    }
        
    void Buffer::put(String* str)
    {
        put(str->str(), str->length());
    }
        
    Reference<String> Buffer::getLine()
    {
        OneChar* data = getData();
        int len = getLineLength();
        if (len == 0)
        {
            return nullptr;
        }
        addReadPos(len);

        if (data[len - 1] == OC '\n')
        {
            len--;
        }
        if (len > 0 && data[len - 1] == OC '\r')
        {
            len--;
        }
        
        return String::createString(data, len);
    }
        
    Reference<String> Buffer::getString(OneInt len)
    {
        OneChar* data = getData();
        if (len == -1)
        {
            len = getDataLength();
        }
        else
        {
            len = std::min(len, (int)getDataLength());
        }
        addReadPos(len);

        return String::createString(data, len);
    }
    
    Reference<String> Buffer::toString()
    {
        return String::createString(getData(), getDataLength());
    }
        
    void Buffer::put(const OneChar* data, OneInt len)
    {
        if (_writePos + len > _capacity)
        {
            resizeCapacity(_writePos + len);
        }
        memcpy(_buf + _writePos, data, len);
        _writePos += len;
    }
       
    void Buffer::resizeCapacity(OneInt capacity)
    {
        if (_capacity >= capacity)
        {
            return;
        }
        while (_capacity < capacity)
        {
            _capacity *= 2;
        }
        OneChar* buf = new OneChar[_capacity];
        memcpy(buf, _buf, _writePos);
        delete[] _buf;
        _buf = buf;
    }
        
    //指向\n后面的字符
    OneInt Buffer::getLineLength()
    {
        OneChar* data = getData();
        OneInt len = getDataLength();
        OneInt off = 0;
        while (off < len)
        {
            if (data[off] == OC '\n')
            {
                off++;
                break;
            }
        }
        return off;
    }
} // namespace One

