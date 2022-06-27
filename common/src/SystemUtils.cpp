#include "SystemUtils.h"
#include <time.h>
#include <dirent.h>
#include <unistd.h>

namespace OneCommon
{
    unsigned int SystemUtils::getMSTick()
    {
        struct timespec time = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &time);
        return time.tv_sec * 1000 + time.tv_nsec / 1000 / 1000;
    }

    unsigned long long SystemUtils::getUSTick()
    {
        struct timespec time = {0, 0};
        clock_gettime(CLOCK_MONOTONIC, &time);
        return (unsigned long long)time.tv_sec * 1000 * 1000 + time.tv_nsec / 1000;
    }

    void SystemUtils::sleep(unsigned int msec)
    {
        usleep(msec * 1000);
    }
}
