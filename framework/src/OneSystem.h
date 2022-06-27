#pragma once
#include "OneObject.h"
#include "Reference.h"
#include "OneInterface.h"

namespace One
{
    class CoRunner : public Interface
    {
    public:
        CoRunner(Object* obj) : Interface(obj)
        {}
    public:
        virtual void run() = 0;
    };

    class System : public Object
    {
    public:
        static void createCoroutine(CoRunner* runner);
        static void co(CoRunner* runner);

        static void yield();
    };
}
