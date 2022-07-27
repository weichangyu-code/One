#pragma once
#include "Common.h"
#include <mutex>
#include <condition_variable>
#include <semaphore.h>


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
