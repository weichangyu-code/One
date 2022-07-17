#pragma once
#include "OneString.h"

namespace One
{
    class StringPool
    {
    public:
        StringPool();

        void setStringArray(const char** stringArray, unsigned int size);
        Reference<String> getString(int index);

    protected:
        const char** _stringArray = nullptr;
        String** _pool = nullptr;
    };

    extern StringPool g_stringPool;
}
