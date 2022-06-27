#pragma once
#include "one/std/OneStdCoRunner.h"
#include "one/std/OneStdObject.h"
#include "one/std/OneStdOut.h"

namespace One {

class OneTestHelloOne;
class OneTestHelloOneMainAnony0 : public OneStdObject, public OneStdCoRunner
{
public:
    OneTestHelloOneMainAnony0() : OneStdCoRunner(this) {}
    Reference<OneTestHelloOne> this1;
    Reference<OneStdString> str;

    void __initVar__();
    static void __initStaticVar__();
    void __construct__(Pointer<OneTestHelloOne> var0, Pointer<OneStdString> var1);
    virtual void __destruct__();
    void __clearVar__();
    void run();
};

class OneTestHelloOneMainAnony0Factory
{
public:
    static Reference<OneTestHelloOneMainAnony0> __createObject__(Pointer<OneTestHelloOne> var0, Pointer<OneStdString> var1)
    {
        auto __var__ = (OneTestHelloOneMainAnony0*)g_objectPool.createObject(sizeof(OneTestHelloOneMainAnony0));
        CALL_CONSTRUCT(__var__, OneTestHelloOneMainAnony0);
        __var__->__initVar__();
        __var__->__construct__(var0, var1);
        return Reference<OneTestHelloOneMainAnony0>(__var__, false, false);
    }

};

}

