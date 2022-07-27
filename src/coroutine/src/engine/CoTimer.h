﻿#pragma once 
#include "Timer.h"
#include "Coroutine.h"

namespace OneCoroutine
{
    class Engine;
    class CoTimer
    {
    public:
        CoTimer();
        CoTimer(Engine* engine);

        void start(const TimerCB& cb, unsigned int delay, unsigned int interval);
        void stop();
        bool isStart();

        Engine* getEngine();

    protected:
        Timer _timer;
        TimerCB _cb;
        Coroutine::Ptr _coroutine;
    };
} // namespace One
