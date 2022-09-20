#include "OneString.h"
#include "ObjectPool.h"
#include "StringUtils.h"
using namespace OneCommon;

namespace One
{
    int String::length()
    {
        if (this == nullptr)
        {
            return 0;
        }
        return _length;
    }
        
    const char* String::str()
    {
        if (this == nullptr)
        {
            return "";
        }
        return _c;
    }
        
    void String::__destruct__()
    {
        _length = 0;
        _c[0] = 0;
    }

    Reference<String> String::createString(unsigned int length)
    {
        String* str = g_objectPool.createObjectT<String>(sizeof(String) + length);
        str->_length = length;
        return Reference<String>(str, false, false);
    }
    
    Reference<String> String::createString(const char* str)
    {
        Reference<String> strRef = createString(strlen(str));
        strcpy(strRef->_c, str);
        return strRef;
    }

    Reference<String> String::valueOf(int v)
    {
        return createString(StringUtils::itoa(v).c_str());
    }

}
