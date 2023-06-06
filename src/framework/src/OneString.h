#pragma once
#include "OneObject.h"
#include "OneIterator.h"
#include "OneIterable.h"

namespace One
{
    #define OC                  (OneChar)
    #define OSTR                (const OneChar*)
    const static OneChar* EMPTY_STRING = OSTR "";

    class StringIterator;
    template<class T> class Array;
    class String : public Object, public Iterable<signed char>
    {
        friend class StringIterator;
        friend class StringBuilder;
    public:
        String();

        //类型转换
        static Reference<String> valueOf(OneInt v);
        static Reference<String> valueOf(OneLong v);
        static Reference<String> valueOf(OneFloat v);
        static Reference<String> valueOf(OneDouble v);

        //打印字符串
        virtual Reference<String> toString();

        //操作符重载
        Reference<String> clone();
        OneBool equal(String* str);
        int  compare(String* str);
        Reference<String> combine(Array<String>* args);
        Reference<String> format(Array<String>* args);
        OneChar get(OneInt index);
        OneChar set(OneInt index, OneChar value);
        virtual Reference<Iterator<OneChar>> iterator();        //也可以认为是For重载

        OneInt length();
        OneChar* data();
        const OneChar* str();
        const OneChar* end();

        Reference<String> substr(OneInt start = 0, OneInt size = -1);
        int find(String* str, OneInt start = 0);
        Reference<String> replace(String* src, String* dst);
        Reference<String> replace(OneChar src, OneChar dst);
        Reference<String> toUpper();
        Reference<String> toLower();

    protected:
        void findEach(const OneChar* src, const function<void(const OneChar* start, const OneChar* find)>& func);
        OneInt _strlen(const OneChar* str);
        const OneChar* _strstr(const OneChar* str, const OneChar* find);
        
    //内部接口
    public:
        virtual void __destruct__();
        
        static Reference<String> createString(OneInt length);
        static Reference<String> createString(const OneChar* str);
        static Reference<String> createString(const OneChar* str, OneInt length);
        static Reference<String> createString(const string& str);

    protected:
        OneInt _length = 0;
        OneChar _c[1] = {0};
    };

    class StringIterator : public Object, public Iterator<signed char>
    {
    public:
        StringIterator() : Iterator<signed char>(this)
        {
        }

        void setData(String* str)
        {
            _strRef = str;
            _data = str->str();
        }

        virtual OneBool hasNext()
        {
            return _data < _strRef->end();
        }

        virtual OneChar next()
        {
            return *(_data++);
        }

    protected:
        Reference<String> _strRef;
        const OneChar* _data = nullptr;
    };
}
