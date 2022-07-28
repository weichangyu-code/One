#pragma once
#include "CoCondition.h"

namespace OneCoroutine
{
    class CoMutex
    {
    public:
        CoMutex();

        bool lock(unsigned int timeout = UINT_MAX);
        void unlock();
        bool isLock();

    protected:
        bool _lock = false;
        CoCondition cond;
    };

    class CoMutexGuard
    {
    public:
        CoMutexGuard(CoMutex& mtx)
            :_mtx(mtx)
        {
            _mtx.lock();
        }
        ~CoMutexGuard()
        {
            _mtx.unlock();
        }

    protected:
        CoMutex& _mtx;
    };

} // namespace One
