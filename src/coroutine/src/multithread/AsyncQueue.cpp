#include "AsyncQueue.h"

namespace OneCoroutine
{
    AsyncQueue::AsyncQueue()
        :_queue(4*1024)
    {

    }

    void AsyncQueue::push(const SimpleFunction& func)
    {
        _queue.push(func);
    }

    void AsyncQueue::run()
    {
        if (_queue.empty())
        {
            return;
        }
        
        SimpleFunction func;
        while (_queue.pop(func))
        {
            func();
        }
    }
        
    bool AsyncQueue::empty()
    {
        return _queue.empty();
    }
}