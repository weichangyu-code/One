#pragma once
#include "Coroutine.h"


namespace OneCoroutine
{
    class CoSystem
    {
    public:
        static void yield();
        static void sleep(unsigned int msec);
        
        static Coroutine::Ptr createCoroutine(const CoroutineRunner& runner);

        static void executeOnMain(const SimpleFunction& func);
    };
} // namespace One

