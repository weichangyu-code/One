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

    Reference<String> String::toString()
    {
        return this;
    }
    
    Reference<String> String::clone()
    {
        return this;
    }
    
    bool String::equal(String* str)
    {
        return compare(str) == 0;
    }
        
    int String::compare(String* str)
    {
        const char* left = this == nullptr ? "" : _c;
        const char* right = str == nullptr ? "" : str->_c;
        return strcmp(left, right);
    }

    // bool String::equal(Object* obj)
    // {
    //     if (this == obj)
    //     {
    //         return true;
    //     }
    //     if (obj == nullptr)
    //     {
    //         return false;
    //     }
    //     if (__class__ != obj->getClass())
    //     {
    //         return false;
    //     }
    //     String* str = (String*)obj;
    //     return strcmp(_c, str->_c) == 0;
    // }

}
