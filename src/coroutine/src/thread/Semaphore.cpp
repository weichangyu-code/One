#include "Semaphore.h"

#ifndef _WIN32
#include <sys/time.h>
#endif

namespace OneCoroutine
{
	Semaphore::Semaphore(unsigned int init)
	{
#ifdef _WIN32
		_sema_h = ::CreateSemaphorephore(NULL, init, UINT_MAX, NULL);
#else
		sem_init(&_sema_h, 0, init);
#endif
	}

	Semaphore::~Semaphore()
	{
#ifdef _WIN32
        ::CloseHandle(_sema_h);
#else
		sem_destroy(&_sema_h);
#endif
	}

	void Semaphore::post()
	{
#ifdef _WIN32
        ::ReleaseSemaphorephore(_sema_h, 1, 0);
#else
		sem_post(&_sema_h);
#endif
	}

#ifndef _WIN32
    void get_off_abstime(struct timespec* abs_time, uint msec_off)
    {
        struct timeval now; 
        gettimeofday(&now, NULL); 
        abs_time->tv_nsec = now.tv_usec * 1000 + (msec_off % 1000) * 1000000; 
        abs_time->tv_sec = now.tv_sec + msec_off / 1000;
        if (abs_time->tv_nsec >= 1000000000)
        {
            abs_time->tv_sec++;
            abs_time->tv_nsec -= 1000000000;
        }
    }
#endif

	bool Semaphore::wait(unsigned int timeout)
	{
#ifdef _WIN32
		return WaitForSingleObject(_sema_h, (timeout == UINT_MAX) ? INFINITE : timeout) == WAIT_OBJECT_0;
#else
		if (timeout == UINT_MAX)
		{
			sem_wait(&_sema_h);
			return true;
		}
		else
		{
			struct timespec abs_time;
			get_off_abstime(&abs_time, timeout);
			return sem_timedwait(&_sema_h, &abs_time) == 0;
		}
#endif
	}

} // namespace OneCoroutine
