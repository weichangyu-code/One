#include "OneTestHelloOneMainAnony0.h"
#include "StringPool.h"
#include "ObjectPool.h"
#include "one/std/OneStdObject.h"
#include "one/std/OneStdOut.h"
#include "one/test/OneTestHelloOne.h"

namespace One {


void OneTestHelloOneMainAnony0::__initVar__()
{
}

void OneTestHelloOneMainAnony0::__initStaticVar__()
{
}

void OneTestHelloOneMainAnony0::__construct__(Pointer<OneTestHelloOne> var0, Pointer<OneStdString> var1)
{
    this->this1 = var0;
    this->str = var1;
}

void OneTestHelloOneMainAnony0::__destruct__()
{
    __clearVar__();
}

void OneTestHelloOneMainAnony0::__clearVar__()
{
    this->this1.clear();
    this->str.clear();
}

void OneTestHelloOneMainAnony0::run()
{
    OneStdOut::println(this->str);
}


}
