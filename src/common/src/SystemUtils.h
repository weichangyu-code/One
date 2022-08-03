#pragma once
#include "Common.h"

namespace OneCommon
{
    class SystemUtils
    {
    public:
        static unsigned int getMSTick();           //毫秒，有循环风险
        static unsigned long long getUSTick();     //微妙
        
        static void sleep(unsigned int msec);
    };
}