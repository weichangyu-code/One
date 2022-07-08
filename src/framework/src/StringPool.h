#pragma once
#include "OneString.h"

namespace One
{
    class StringPool
    {
    public:
        StringPool();

        void reserve(unsigned int size);
        void addString(int index, const char* str);
        Reference<String> getString(int index);

    protected:
        String** pool;
    };

    extern StringPool g_stringPool;
}
