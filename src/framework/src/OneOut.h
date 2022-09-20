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
        static void print(String* str, Array<String>* args);
        static void println(String* str, Array<String>* args);

    protected:
        static string formatString(String* str, Array<String>* args);
    };
}
