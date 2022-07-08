#include "OneOut.h"
#include "OneString.h"
#include <stdio.h>
#include "engine/CoStdOut.h"
using namespace OneCoroutine;

namespace One
{
    void Out::print(String* str)
    {
        if (str == nullptr)
        {
            return;
        }
        CoStdOut::print("%s", str->_c);
    }
        
    void Out::println(String* str)
    {
        if (str == nullptr)
        {
            return;
        }
        CoStdOut::print("%s\n", str->_c);
    }

} // namespace One

