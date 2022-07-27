#pragma once
#include "Common.h"
#include "LockFreeQueue.h"
#include <mutex>
using namespace std;

namespace OneCoroutine
{
    class AsyncQueue
    {
    public:
        AsyncQueue();

    public:
        void push(const SimpleFunction& func);
        bool empty();

        void run();             //接收线程执行

    protected:
        LockFreeQueue<mutex, false_mutex, SimpleFunction> _queue;
    };
} // namespace OneCoroutine

