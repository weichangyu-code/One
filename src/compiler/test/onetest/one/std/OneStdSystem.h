#pragma once
#include "ObjectPool.h"
#include "OneSystem.h"
#include "one/std/OneStdCoRunner.h"
#include "one/std/OneStdObject.h"

namespace One {

typedef System OneStdSystem;
class OneStdSystemFactory
{
public:
    static Reference<OneStdSystem> __createObject__()
    {
        auto __var__ = (OneStdSystem*)g_objectPool.createObject(sizeof(OneStdSystem));
        CALL_CONSTRUCT(__var__, OneStdSystem);
        return Reference<OneStdSystem>(__var__, false, false);
    }

};

}
