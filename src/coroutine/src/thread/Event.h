#pragma once
#include "Common.h"
#include <mutex>
#include <condition_variable>
#include "Atomic.h"

/*
Event signal内部有很大概率锁冲突，性能不是很好
*/

namespace OneCoroutine
{
    class Event
    {
    public:
		Event(bool autoReset = true);
		~Event();

		void signal(bool signal = true);
		bool wait(unsigned int timeout = UINT_MAX);

    protected:
        mutex _mtx;
        condition_variable _cond;
        bool _autoReset = true;
        bool _signal = false;
    };

}
