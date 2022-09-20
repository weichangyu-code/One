#pragma once
#include "OneObject.h"

namespace One
{
    class String : public Object
    {
    public:
        int length();
        const char* str();

        static Reference<String> valueOf(int v);
        
    //内部接口
    public:
        virtual void __destruct__();
        
        static Reference<String> createString(unsigned int length);
        static Reference<String> createString(const char* str);

    protected:
        unsigned int _length = 0;
        char _c[1] = {0};
    };
}
