#include "OneOut.h"
#include <stdio.h>
#include "engine/CoStdOut.h"
#include "StringUtils.h"
#include <string>
using namespace OneCoroutine;
using namespace OneCommon;
using namespace std;

namespace One
{
    string Out::formatString(String* str, Array<String>* args)
    {
        string dst;
        dst.reserve(str->length() * 2);

        const char* str2 = str->str();
        int i = 0;
        while (1)
        {
            const char* find = strstr(str2, "{}");
            if (find == nullptr)
            {
                break;
            }
            dst += string(str2, find - str2);
            str2 = find + 2;
            
            if (i < args->length())
            {
                dst += args->at(i)->str();
            }
            else
            {
                dst += "{}";    //如果没有替换，还是保留
            }
            i++;
        }
        dst += str2;
        return dst;
    }

    void Out::print(String* str, Array<String>* args)
    {
        if (str == nullptr)
        {
            return;
        }
        if (args->length() == 0)
        {
            CoStdOut::print("%s", str->str());
        }
        else
        {
            string dst = formatString(str, args);
            CoStdOut::print("%s", dst.c_str());
        }
    }
        
    void Out::println(String* str, Array<String>* args)
    {
        if (str == nullptr)
        {
            return;
        }
        if (args->length() == 0)
        {
            CoStdOut::print("%s\n", str->str());
        }
        else
        {
            string dst = formatString(str, args);
            CoStdOut::print("%s\n", dst.c_str());
        }
    }

} // namespace One

