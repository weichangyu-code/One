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
    void Out::print(String* str)
    {
        if (str == nullptr)
        {
            return;
        }
        CoStdOut::print("%s", str->str());
    }
        
    void Out::println(String* str)
    {
        if (str == nullptr)
        {
            return;
        }
        CoStdOut::print("%s\n", str->str());
    }

} // namespace One

