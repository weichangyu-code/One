#pragma once
#include "OneObject.h"

namespace One
{
    class Runner : public Interface
    {
    public:
        Runner(Object* obj) : Interface(obj)
        {}
    public:
        virtual void run() = 0;
    };

    class System : public Object
    {
    public:
        static void createCoroutine(Runner* runner);
        static void co(Runner* runner);

        static void yield();
    };
}
