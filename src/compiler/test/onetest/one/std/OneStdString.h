#pragma once
#include "ObjectPool.h"
#include "OneString.h"
#include "one/std/OneStdObject.h"

namespace One {

typedef String OneStdString;
class OneStdStringFactory
{
public:
    static Reference<OneStdString> __createObject__()
    {
        auto __var__ = (OneStdString*)g_objectPool.createObject(sizeof(OneStdString));
        CALL_CONSTRUCT(__var__, OneStdString);
        return Reference<OneStdString>(__var__, false, false);
    }

};

}
