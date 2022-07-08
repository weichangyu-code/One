#pragma once
#include "CoCondition.h"

namespace OneCoroutine
{
    class CoMutex
    {
    public:
        CoMutex();
        CoMutex(Engine* engine);

        bool lock(unsigned int timeout = UINT_MAX);
        void unlock();
        bool isLock();

    protected:
        bool _lock = false;
        CoCondition cond;
    };
} // namespace One
