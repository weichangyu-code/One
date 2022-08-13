#include "../src/engine/Engine.h"
#include "../src/engine/CoCondition.h"
#include "../src/engine/CoMutex.h"
#include "../src/engine/CoStdOut.h"
#include "../src/engine/CoFile.h"
#include "SystemUtils.h"
#include <stdio.h>
using namespace OneCommon;
using namespace OneCoroutine;

int main()
{
    Engine engine;
    engine.createCoroutine([](Coroutine* co){

        unsigned int start = SystemUtils::getMSTick();

        CoFile file;
        file.open("./a.out");
        char buf[4*1024];
        for (int i = 0;i < 100000;i++)
        {
            file.write(buf, sizeof(buf));
        }
        file.close();

        CoStdOut::print("use time=%d\n", SystemUtils::getMSTick() - start);
    });
    engine.run();

    return 0;
}