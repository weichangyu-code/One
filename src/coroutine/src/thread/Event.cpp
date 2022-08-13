#include "Event.h"

namespace OneCoroutine
{
    Event::Event(bool autoReset)
    {
        this->_autoReset = autoReset;
    }
    Event::~Event()
    {

    }

    void Event::signal(bool signal)
    {
        unique_lock<mutex> l(_mtx);
        if (_signal != signal)
        {
            _signal = signal;
            if (_signal)
            {
                l.unlock();
                if (_autoReset)
                {
                    _cond.notify_one();
                }
                else
                {
                    _cond.notify_all();
                }
            }
        }
    }

    bool Event::wait(unsigned int timeout)
    {
        unique_lock<mutex> l(_mtx);

        bool ret = true;
        if (_signal == false)
        {
            if (timeout == UINT_MAX)
            {
                _cond.wait(l);
            }
            else
            {
                ret = _cond.wait_for(l, std::chrono::milliseconds(timeout)) == cv_status::no_timeout;
            }
        }

        if (ret && _autoReset)
        {
            _signal = false;
        }

        return ret;
    }

}