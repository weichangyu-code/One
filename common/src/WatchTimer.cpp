#include "WatchTimer.h"
#include "SystemUtils.h"

namespace OneCommon
{
    WatchTimer::WatchTimer(unsigned int interval)
    {
        this->interval = interval;
        this->lastTick = SystemUtils::getMSTick();
    }
        
    void WatchTimer::setInterval(unsigned int interval)
    {
        this->interval = interval;
    }
        
    bool WatchTimer::check()
    {
        unsigned int tick = SystemUtils::getMSTick();
        if ((tick - lastTick) >= interval)
        {
            lastTick = tick;
            return true;
        }
        return false;
    }
        
    void WatchTimer::active()
    {
        lastTick = SystemUtils::getMSTick();
    }
}