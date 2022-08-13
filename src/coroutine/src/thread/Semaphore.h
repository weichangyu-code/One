#pragma once
#include "Common.h"

#ifndef _WIN32
#include <semaphore.h>
#endif

/**
 * 在LINUX下，Semaphore性能高于Event
 * 
 * */

namespace OneCoroutine
{
    class Semaphore
    {
	public:
		Semaphore(unsigned int init = 0);
		~Semaphore();

		void post();
		bool wait(unsigned int timeout = UINT_MAX);

	private:
#ifdef _WIN32
		HANDLE _sema_h;
#else
		sem_t _sema_h;
#endif
    };
} // namespace OneCoroutine
