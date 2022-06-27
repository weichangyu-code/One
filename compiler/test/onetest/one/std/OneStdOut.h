#pragma once
#include "ObjectPool.h"
#include "OneOut.h"
#include "one/std/OneStdObject.h"
#include "one/std/OneStdString.h"

namespace One {

typedef Out OneStdOut;
class OneStdOutFactory
{
public:
    static Reference<OneStdOut> __createObject__()
    {
        auto __var__ = (OneStdOut*)g_objectPool.createObject(sizeof(OneStdOut));
        CALL_CONSTRUCT(__var__, OneStdOut);
        return Reference<OneStdOut>(__var__, false, false);
    }

};

}
