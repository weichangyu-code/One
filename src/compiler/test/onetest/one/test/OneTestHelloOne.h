#pragma once
#include "one/std/OneStdObject.h"
#include "one/std/OneStdString.h"
#include "one/std/OneStdSystem.h"

namespace One {

class OneTestHelloOneMainAnony0;
class OneTestHelloOne : public OneStdObject
{
public:
    OneTestHelloOne() {}

    void __initVar__();
    static void __initStaticVar__();
    void main();
    void __construct__();
    virtual void __destruct__();
    void __clearVar__();
};

class OneTestHelloOneFactory
{
public:
    static Reference<OneTestHelloOne> __createObject__()
    {
        auto __var__ = (OneTestHelloOne*)g_objectPool.createObject(sizeof(OneTestHelloOne));
        CALL_CONSTRUCT(__var__, OneTestHelloOne);
        __var__->__initVar__();
        __var__->__construct__();
        return Reference<OneTestHelloOne>(__var__, false, false);
    }

};

}

