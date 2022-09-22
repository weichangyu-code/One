#include "OneString.h"
#include "ObjectPool.h"
#include "StringUtils.h"
#include "OneArray.h"
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
        
    Reference<String> String::combine(Array<String>* strs)
    {
        //计算长度
        unsigned int len = length();
        for (int i = 0;i < strs->length();i++)
        {
            len += strs->indexOf(i)->length();
        }

        //字符串叠加
        Reference<String> strRef = createString(len);
        char* buf = strRef->_c;
        strcpy(buf, str());
        buf += _length;
        for (int i = 0;i < strs->length();i++)
        {
            strcpy(buf, strs->indexOf(i)->str());
            buf += strs->indexOf(i)->length();
        }

        return strRef;
    }
}
