#pragma once
#include "OneObject.h"

namespace One
{
    class String : public Object
    {
    public:
        int length();
        const char* str();
        
    //内部接口
    public:
        virtual void __destruct__();
        void setData(const char* str);
        static Reference<String> createString(unsigned int length);

    public:
        unsigned int _length = 0;
        char _c[1];
    };
}
