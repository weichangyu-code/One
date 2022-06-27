#include "Timer.h"
#include "TimerManager.h"
 
namespace OneCoroutine
{
    Timer::Timer(TimerManager* manager)
    {
        this->mananger = manager;
        this->iter = manager->timerMap.end();
    }
        
    Timer::~Timer()
    {
        stop();
    }
    
    void Timer::start(const TimerCB& cb, unsigned int delay, unsigned int interval)
    {
        stop();

        this->cb = cb;
        this->interval = interval;
        this->_start = true;
        mananger->startTimer(this, delay);
    }
        
    void Timer::stop()
    {
        if (_start)
        {
            _start = false;
            mananger->stopTimer(this);
        }
    }
        
    bool Timer::isStart()
    {
        return _start;
    }
        
    Engine* Timer::getEngine()
    {
        return mananger->getEngine();
    }
    
    void Timer::onTime()
    {
        if (interval > 0)
        {
            mananger->startTimer(this, interval);
        }
        else
        {
            //自动结束
            _start = false;
        }

        this->cb();
    }

} // namespace One
