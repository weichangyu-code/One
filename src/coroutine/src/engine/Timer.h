#pragma once
#include "List.h"
#include <map>
#include <functional>
using namespace OneCommon;

namespace OneCoroutine
{
    class Timer;
    class TimerManager;
    class Engine;

    typedef std::multimap<unsigned int, Timer*> TimerMap;
    typedef std::function<void()> TimerCB;

    class Timer
    {
        friend class TimerManager;
    public:
        Timer(TimerManager* manager);
        ~Timer();

        void start(const TimerCB& cb, unsigned int delay, unsigned int interval);
        void stop();
        bool isStart();

        Engine* getEngine();

    protected:
        void onTime();

    protected:
        TimerManager* mananger;
        TimerCB cb;

        bool _start = false;
        unsigned int interval = 0;

        unsigned int activeTime = 0;
        TimerMap::iterator iter;
        ListNode node;
    };

} // namespace One
