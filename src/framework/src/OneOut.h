#pragma once
#include "OneObject.h"
#include "Reference.h"
#include "OneArray.h"
#include "OneString.h"

namespace One
{
    class String;
    class Out : public Object
    {
    public:
        static void print(String* str);                     //移除format参数，直接利用字符串本身的%%操作符
        static void println(String* str);
    };
}
