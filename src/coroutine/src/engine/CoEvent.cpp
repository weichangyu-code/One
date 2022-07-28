#include "CoEvent.h"

namespace OneCoroutine
{
    CoEvent::CoEvent(bool autoReset)
    {
        _autoReset = autoReset;
    }

	void CoEvent::signal(bool sig)
    {
        _signal = sig;
        if (_signal)
        {
            if (_autoReset)
            {
                if (cond.haveWaiter())
                {
                    //激活一个，状态变成false
                    _signal = false;
                    cond.active(false);
                }
            }
            else
            {
                //非自动，激活所有
                cond.active(true);
            }
        }
    }

    bool CoEvent::wait(unsigned int timeout)
    {
        if (_signal)
        {
            if (_autoReset)
            {
                _signal = false;
            }
            return true;
        }
        else
        {
            return cond.wait(timeout);
        }
    }
        
    bool CoEvent::isSignal()
    {
        return _signal;
    }
        
    bool CoEvent::haveWaiter()
    {
        return cond.haveWaiter();
    }
    
} // namespace One
