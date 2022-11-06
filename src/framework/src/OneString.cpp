#include "OneString.h"
#include "ObjectPool.h"
#include "StringUtils.h"
#include "OneArray.h"
using namespace OneCommon;

namespace One
{
    String::String() : Iterable<signed char>(this)
    {

    }

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
        
    const char* String::end()
    {
        if (this == nullptr)
        {
            return "";
        }
        return _c + _length;
    }

    char* String::data()
    {
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
        return Reference<String>(str, true, false);
    }
    
    Reference<String> String::createString(const char* str)
    {
        Reference<String> strRef = createString((unsigned int)strlen(str));
        strcpy(strRef->_c, str);
        return strRef;
    }

    Reference<String> String::valueOf(int v)
    {
        char buf[32];
        StringUtils::itoa(v, buf);
        return createString(buf);
    }
    
    Reference<String> String::valueOf(signed char c)
    {
        char buf[2];
        buf[0] = c;
        buf[1] = 0;
        return createString(buf);
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
        return strcmp(this->str(), str->str());
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
        char* buf = strRef->data();
        strcpy(buf, str());
        buf += this->length();
        for (int i = 0;i < strs->length();i++)
        {
            strcpy(buf, strs->indexOf(i)->str());
            buf += strs->indexOf(i)->length();
        }

        return strRef;
    }
        
    Reference<String> String::substr(int start, int size)
    {
        start = min(max(0, start), length());
        if (size < 0 && (size + start) > length())
        {
            size = length() - start;
        }
        Reference<String> strRef = createString(size);
        memcpy(strRef->data(), this->str() + start, size);
        return strRef;
    }
        
    int String::find(String* str, int start)
    {
        start = min(max(0, start), length());
        const char* f = strstr((char*)this->str() + start, str->str());
        if (f == nullptr)
        {
            return 0;
        }
        else
        {
            return (int)(f - this->str());
        }
    }
        
    void String::findEach(String* src, const function<void(const char* start, const char* find)>& func)
    {
        const char* start = this->str();
        while (1)
        {
            char* find = strstr((char*)start, src->str());
            if (find == nullptr)
            {
                break;
            }
            func(start, find);
            start = find + src->length();
        }
        func(start, this->end());
    }
        
    Reference<String> String::replace(String* src, String* dst)
    {
        //
        int findNum = -1;
        findEach(src, [&findNum](const char* start, const char* find) {
            findNum++;
        });
        if (findNum == 0)
        {
            return this;
        }

        int newLen = this->length() + (dst->length() - src->length()) * findNum;
        Reference<String> strRef = createString(newLen);
        char* writePos = strRef->data();

        findEach(src, [&writePos, dst](const char* start, const char* find) {
            memcpy(writePos, start, find - start);
            writePos += find - start;
            
            if (*find != 0)
            {
                memcpy(writePos, dst->str(), dst->length());
                writePos += dst->length();
            }
        });
        return strRef;
    }
        
    Reference<String> String::replace(signed char src, signed char dst)
    {
        Reference<String> strRef = createString(this->str());
        signed char* data = (signed char*)strRef->data();
        for (int i = 0;i < length();i++)
        {
            if (data[i] == src)
            {
                data[i] = dst;
            }
        }
        return strRef;
    }
        
    Reference<String> String::toUpper()
    {
        Reference<String> strRef = createString(this->str());
        char* data = strRef->data();
        for (int i = 0;i < length();i++)
        {
            if (data[i] >= 'a' && data[i] <= 'z')
            {
                data[i] = data[i] - 'a' + 'A';
            }
        }
        return strRef;
    }
        
    Reference<String> String::toLower()
    {
        Reference<String> strRef = createString(this->str());
        char* data = strRef->data();
        for (int i = 0;i < length();i++)
        {
            if (data[i] >= 'A' && data[i] <= 'Z')
            {
                data[i] = data[i] - 'A' + 'a';
            }
        }
        return strRef;
    }
    
    Reference<Iterator<signed char>> String::iterator()
    {
        Reference<StringIterator> iterRef = g_objectPool.createObjectR<StringIterator>();
        iterRef->setData(this);
        return convertInterfaceReference<StringIterator, Iterator<signed char>>(iterRef);
    }
}
