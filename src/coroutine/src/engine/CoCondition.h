#pragma once
#include "List.h"
#include <climits>
using namespace OneCommon;

namespace OneCoroutine
{
    class Engine;
    class CoCondition
    {
        friend class Engine;
    public:
        CoCondition();
        CoCondition(Engine* engine);

        bool wait(unsigned int timeout = UINT_MAX);
        bool active(bool all = false);      //激活一个协程还是所有协程

        bool haveWaiter();

    protected:
        Engine* engine;
        ListHead waitCos;           //等待这个时间的协程
    };
} // namespace One
