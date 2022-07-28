#include "OneBuffer.h"

namespace One
{
    Buffer::Buffer(int length)
    {
        _buf = new unsigned char[length];
        _length = length;
    }
    Buffer::~Buffer()
    {
        delete[] _buf;
    }
} // namespace One

