#include "OneTestHelloOne.h"
#include "StringPool.h"
#include "ObjectPool.h"
#include "one/std/OneStdObject.h"
#include "one/std/OneStdString.h"
#include "one/std/OneStdSystem.h"
#include "one/test/OneTestHelloOneMainAnony0.h"

namespace One {


void OneTestHelloOne::__initVar__()
{
}

void OneTestHelloOne::__initStaticVar__()
{
}

void OneTestHelloOne::main()
{
    Reference<OneStdString> str;
    str = g_stringPool.getString(0);
    OneStdSystem::co((OneTestHelloOneMainAnony0Factory::__createObject__(Pointer<OneTestHelloOne>(this, true), str)));
}

void OneTestHelloOne::__construct__()
{
}

void OneTestHelloOne::__destruct__()
{
    __clearVar__();
}

void OneTestHelloOne::__clearVar__()
{
}


}
