#include "OneString.h"
#include "ObjectPool.h"

namespace One
{
    int String::length()
    {
        return _length;
    }

    Reference<String> String::createString(unsigned int length)
    {
        String* str = g_objectPool.createObjectT<String>(sizeof(String) + length);
        str->_length = length;
        return Reference<String>(str, false, false);
    }
}
