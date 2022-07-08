#pragma once
#include "OneObject.h"
#include "Reference.h"

namespace One
{
    class String;
    class Out : public Object
    {
    public:
        static void print(String* str);
        static void println(String* str);
    };
}
