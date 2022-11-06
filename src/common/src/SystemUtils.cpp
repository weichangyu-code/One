#include "SystemUtils.h"
#include <time.h>
#include <thread>

namespace OneCommon
{
    unsigned int SystemUtils::getMSTick()
    {
        return (unsigned int)(getUSTick() / 1000);
    }

    unsigned long long SystemUtils::getUSTick()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    void SystemUtils::sleep(unsigned int msec)
    {
        //usleep(msec * 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }
    
    void SystemUtils::yield()
    {
        std::this_thread::yield();
    }
}
