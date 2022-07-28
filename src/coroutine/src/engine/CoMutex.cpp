#include "CoMutex.h"

namespace OneCoroutine
{
    CoMutex::CoMutex()
    {

    }

    bool CoMutex::lock(unsigned int timeout)
    {
        while (_lock)
        {
            if (cond.wait(timeout) == false)
            {
                return false;
            }
        }
        _lock = true;
        return true;
    }

    void CoMutex::unlock()
    {
        if (_lock)
        {
            _lock = false;
            cond.active();
        }
    }
        
    bool CoMutex::isLock()
    {
        return _lock;
    }
    
} // namespace One
