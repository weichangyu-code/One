#include "one/test/OneTestHelloOne.h"
#include "StringPool.h"
#include "one/test/OneTestHelloOne.h"
#include "one/test/OneTestHelloOneMainAnony0.h"
#include "engine/Engine.h"

int main()
{
    One::g_stringPool.reserve(1);
    One::g_stringPool.addString(0, "hello one");

    int ret = 0;
    OneCoroutine::Engine engine;
    engine.createCoroutine([&ret](OneCoroutine::Coroutine* co) {
        One::OneTestHelloOne::__initStaticVar__();
        One::OneTestHelloOneMainAnony0::__initStaticVar__();

        auto app = One::OneTestHelloOneFactory::__createObject__();
        app->main();
    });
    engine.run();
    return ret;
}
