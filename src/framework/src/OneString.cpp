#include "OneString.h"
#include "ObjectPool.h"
#include "StringUtils.h"
#include "OneArray.h"
#include "OneBuffer.h"
using namespace OneCommon;

namespace One
{
    String::String() : Iterable<signed char>(this)
    {

    }

    OneInt String::length()
    {
        if (this == nullptr)
        {
            return 0;
        }
        return _length;
    }
        
    const OneChar* String::str()
    {
        if (this == nullptr)
        {
            return EMPTY_STRING;
        }
        return _c;
    }
        
    const OneChar* String::end()
    {
        if (this == nullptr)
        {
            return EMPTY_STRING;
        }
        return _c + _length;
    }

    OneChar* String::data()
    {
        return _c;
    }
        
    void String::__destruct__()
    {
        _length = 0;
        _c[0] = 0;
    }

    Reference<String> String::createString(OneInt length)
    {
        String* str = g_objectPool.createObjectT<String>(sizeof(String) + length);
        str->_length = length;
        return Reference<String>(str, true, false);
    }
    
    Reference<String> String::createString(const OneChar* str)
    {
        Reference<String> strRef = createString((OneInt)strlen((const char*)str));
        memcpy(strRef->_c, str, strRef->_length);
        return strRef;
    }
    
    Reference<String> String::createString(const OneChar* str, OneInt length)
    {
        Reference<String> strRef = createString(length);
        memcpy(strRef->_c, str, length);
        strRef->_c[length] = 0;
        return strRef; 
    }
    
    Reference<String> String::createString(const string& str)
    {
        return createString((const OneChar*)str.c_str());
    }

    Reference<String> String::valueOf(OneInt v)
    {
        return createString(StringUtils::itoa(v));
    }
    
    Reference<String> String::valueOf(OneLong v)
    {
        return createString(StringUtils::ltoa(v));
    }
    
    Reference<String> String::valueOf(OneFloat v)
    {
        return createString(StringUtils::ftoa(v));
    }
    
    Reference<String> String::valueOf(OneDouble v)
    {
        return createString(StringUtils::dtoa(v));
    }

    Reference<String> String::toString()
    {
        return this;
    }

    Reference<String> String::clone()
    {
        return this;
    }
    
    OneBool String::equal(String* str)
    {
        return compare(str) == 0;
    }
        
    int String::compare(String* str)
    {
        return strcmp((const char*)this->str(), (const char*)str->str());
    }
        
    Reference<String> String::combine(Array<String>* args)
    {
        //计算长度
        OneInt len = this->length();
        for (String* arg : *args)
        {
            len += arg->length();
        }

        //字符串叠加
        Buffer buf(len);
        buf.put(this);
        for (String* arg : *args)
        {
            buf.put(arg);
        }

        return buf.toString();
    }
        
    Reference<String> String::format(Array<String>* args)
    {
        OneInt len = this->length();
        for (auto arg : *args)
        {
            len += arg->length();
        }

        //搜索{}，然后替换
        Buffer buf(len);
        int i = 0;
        findEach(OSTR"{}", [&i, &buf, args] (const OneChar* start, const OneChar* find) 
        {
            buf.put(start, (OneInt)(find - start));
            if (*find == 0)
            {
                return;
            }

            if (i < args->length())
            {
                buf.put(args->get(i));
            }
            else
            {
                //如果没参数，变成空
                //buf.put(OSTR"{}", 2);
            }
            i++;
        });
        return buf.toString();
    }
        
    OneChar String::get(OneInt index)
    {
        if (index < 0 || index >= _length)
        {
            return 0;
        }
        else
        {
            return _c[index];
        }
    }
        
    OneChar String::set(OneInt index, OneChar value)
    {
        if (index < 0 || index >= _length)
        {
            return 0;
        }
        else
        {
            _c[index] = value;
            return value;
        }
    }
        
    Reference<String> String::substr(OneInt start, OneInt size)
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
        
    int String::find(String* str, OneInt start)
    {
        start = min(max(0, start), this->length());
        const OneChar* f = _strstr(this->str() + start, str->str());
        if (f == nullptr)
        {
            return 0;
        }
        else
        {
            return (int)(f - this->str());
        }
    }
        
    void String::findEach(const OneChar* src, const function<void(const OneChar* start, const OneChar* find)>& func)
    {
        OneInt srcLen = _strlen(src);
        const OneChar* start = this->str();
        while (1)
        {
            const OneChar* find = _strstr(start, src);
            if (find == nullptr)
            {
                break;
            }
            func(start, find);
            start = find + srcLen;
        }
        func(start, this->end());
    }
        
    OneInt String::_strlen(const OneChar* str)
    {
        if (str == nullptr)
        {
            return 0;
        }
        return (OneInt)strlen((const char*)str);
    }
        
    const OneChar* String::_strstr(const OneChar* str, const OneChar* find)
    {
        return (const OneChar*)strstr((const char*)str, (const char*)find);
    }
        
    Reference<String> String::replace(String* src, String* dst)
    {
        Buffer buf(this->length() + 16);
        findEach(src->str(), [&buf, dst](const OneChar* start, const OneChar* find) 
        {
            buf.put(start, (OneInt)(find - start));
            if (*find != 0)
            {
                buf.put(dst);
            }
        });
        return buf.toString();
    }
        
    Reference<String> String::replace(OneChar src, OneChar dst)
    {
        Reference<String> strRef = createString(this->str());
        OneChar* data = (signed char*)strRef->data();
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
        OneChar* data = strRef->data();
        for (int i = 0;i < length();i++)
        {
            if (data[i] >= OC 'a' && data[i] <= OC 'z')
            {
                data[i] = data[i] - OC 'a' + OC 'A';
            }
        }
        return strRef;
    }
        
    Reference<String> String::toLower()
    {
        Reference<String> strRef = createString(this->str());
        OneChar* data = strRef->data();
        for (int i = 0;i < length();i++)
        {
            if (data[i] >= OC 'A' && data[i] <= OC 'Z')
            {
                data[i] = data[i] - OC 'A' + OC 'a';
            }
        }
        return strRef;
    }
    
    Reference<Iterator<OneChar>> String::iterator()
    {
        Reference<StringIterator> iterRef = g_objectPool.createObjectR<StringIterator>();
        iterRef->setData(this);
        return convertInterfaceReference<StringIterator, Iterator<OneChar>>(iterRef);
    }
}
