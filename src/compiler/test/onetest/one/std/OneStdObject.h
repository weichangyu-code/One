#pragma once
#include "ObjectPool.h"
#include "OneObject.h"

namespace One {

typedef Object OneStdObject;
class OneStdObjectFactory
{
public:
    static Reference<OneStdObject> __createObject__()
    {
        auto __var__ = (OneStdObject*)g_objectPool.createObject(sizeof(OneStdObject));
        CALL_CONSTRUCT(__var__, OneStdObject);
        return Reference<OneStdObject>(__var__, false, false);
    }

};

}
