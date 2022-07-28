#pragma once
#include "Timer.h"

namespace OneCoroutine
{
    class TimerManager
    {
        friend class Timer;
    public:
        TimerManager();

        void startTimer(Timer* timer, unsigned int delay);
        void stopTimer(Timer* timer);

        void run();

    protected:
        unsigned int getSystemTime();
        void callOnTime(ListHead& head);
        void roll(unsigned int time);
        void rollSecondArr(ListHead& head, unsigned int time);
        void rollTimerMap(unsigned int time);

    protected:
        unsigned int curTime;               //当前时间
        ListHead microSecArr[1024];
        ListHead secondArr[1024];
        TimerMap timerMap;
    };
} // namespace One

