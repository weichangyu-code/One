#include "../src/engine/Engine.h"
#include "../src/engine/CoCondition.h"
#include "../src/engine/CoMutex.h"
#include "../src/engine/CoSystem.h"
#include "SystemUtils.h"
#include <stdio.h>
using namespace OneCommon;
using namespace OneCoroutine;

int main()
{
    unsigned int start = SystemUtils::getMSTick();
    int switchCount = 0;

    Engine engine;
    engine.createCoroutine([&switchCount](Coroutine* co){
        for (int i = 0;i < 10000000;i++)
        {
            switchCount++;
            CoSystem::yield();
            //co->yield();
        }
    });
    engine.createCoroutine([&switchCount](Coroutine* co){
        for (int i = 0;i < 10000000;i++)
        {
            switchCount++;
            CoSystem::yield();
            //co->yield();
        }
    });
    engine.run();

    printf("use time %d i=%d\n", SystemUtils::getMSTick() - start, switchCount);

    return 0;
}