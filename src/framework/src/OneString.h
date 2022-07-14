#pragma once
#include "OneObject.h"

namespace One
{
    class String : public Object
    {
    public:
        int length();
        
        static Reference<String> createString(unsigned int length);

    public:
        unsigned int _length = 0;
        char _c[1];
    };
}
