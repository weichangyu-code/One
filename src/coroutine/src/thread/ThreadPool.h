#pragma once
#include "Common.h"
#include "LockFreeQueue.h"
#include "Semaphore.h"
#include "Atomic.h"

namespace OneCoroutine
{
    class ThreadPool
    {
    public:
        ThreadPool();
        void setOption(unsigned int minThreads, unsigned int maxThreads);

        void execute(const SimpleFunction& func);

    protected:
        void createThread(bool core);
        bool shouldExitThread();

    protected:
        unsigned int maxThreads = 10;
        unsigned int minThreads = 3;
        unsigned int threadTimeout = 60*1000;

        AtomicInt threadNum;
        AtomicInt queueNum;
        unsigned int busyTime = 0;

        Semaphore sema;
        LockFreeQueue<mutex, mutex, SimpleFunction> queue;
    };
}
