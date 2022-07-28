#pragma once
#include "CoCondition.h"

namespace OneCoroutine
{
    class CoEvent
    {
    public:
        CoEvent(bool autoReset = true);

		void signal(bool sig = true);
        bool wait(unsigned int timeout = UINT_MAX);
        bool isSignal();
        bool haveWaiter();

    protected:
        bool _autoReset = true;
        bool _signal = false;
        CoCondition cond;
    };
} // namespace One
