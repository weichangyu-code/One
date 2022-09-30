#pragma once
#include "OneObject.h"
#include "OneIterator.h"
#include "OneIterable.h"

namespace One
{
    class StringIterator;
    template<class T> class Array;
    class String : public Object, public Iterable<signed char>
    {
        friend class StringIterator;
    public:
        String();

        int length();
        char* data();
        const char* str();
        const char* end();

        Reference<String> substr(int start = 0, int size = -1);
        int find(String* str, int start = 0);
        Reference<String> replace(String* src, String* dst);
        Reference<String> replace(signed char src, signed char dst);
        Reference<String> toUpper();
        Reference<String> toLower();

        static Reference<String> valueOf(int v);
        static Reference<String> valueOf(signed char c);
        
        virtual Reference<String> toString();
        Reference<String> clone();
        bool equal(String* str);
        int  compare(String* str);
        Reference<String> combine(Array<String>* strs);
        
        virtual Reference<Iterator<signed char>> iterator();

    protected:
        void findEach(String* src, const function<void(const char* start, const char* find)>& func);
        
    //内部接口
    public:
        virtual void __destruct__();
        
        static Reference<String> createString(unsigned int length);
        static Reference<String> createString(const char* str);

    protected:
        unsigned int _length = 0;
        char _c[1] = {0};
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

        virtual bool hasNext()
        {
            return _data < _strRef->end();
        }

        virtual signed char next()
        {
            return (signed char)*(_data++);
        }

    protected:
        Reference<String> _strRef;
        const char* _data = nullptr;
    };
}
