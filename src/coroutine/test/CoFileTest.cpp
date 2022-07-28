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
        CoFile file;
        file.open("./a.out");
        file.write("hello world\n", 13);
        file.close();
        char c[32];
        file.open("./a.out");
        int ret = file.read(c, sizeof(c));
        ret = 0;
    });
    engine.run();

    return 0;
}