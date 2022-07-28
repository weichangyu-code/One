#include "OneString.h"
#include "ObjectPool.h"
#include "StringUtils.h"
using namespace OneCommon;

namespace One
{
    int String::length()
    {
        return _length;
    }
        
    const char* String::str()
    {
        return _c;
    }
        
    void String::__destruct__()
    {
        _length = 0;
        _c[0] = 0;
    }
        
    void String::setData(const char* str)
    {
        StringUtils::strncpy_z(_c, str, _length + 1);
    }

    Reference<String> String::createString(unsigned int length)
    {
        String* str = g_objectPool.createObjectT<String>(sizeof(String) + length);
        str->_length = length;
        return Reference<String>(str, false, false);
    }
}
