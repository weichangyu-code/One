#include "../src/engine/Engine.h"
#include "../src/engine/CoCondition.h"
#include "../src/engine/CoMutex.h"
#include "../src/engine/CoStdOut.h"
#include "SystemUtils.h"
#include <stdio.h>
using namespace OneCommon;
using namespace OneCoroutine;

int main()
{
    Engine engine;
    CoMutex mutex;
    CoCondition cond;
    engine.createCoroutine([&engine, &mutex, &cond](Coroutine* co){
        list<Coroutine::Ptr> subCos;
        for (int i = 0;i < 500;i++)
        {
            Coroutine::Ptr subCo = engine.createCoroutine([i, &mutex, &cond](Coroutine* co)
                {
                    CoStdOut::print("SubCo %d start.time=%u\n", i, SystemUtils::getMSTick());

                    mutex.lock();
                    bool timeout = cond.wait(1) == false;
                    mutex.unlock();
                    
                    CoStdOut::print("SubCo %d end timeout=%d.time=%u\n", i, timeout, SystemUtils::getMSTick());
                });
            subCos.push_back(subCo);
        }
        int i = 0;
        for (auto subCo : subCos)
        {
            subCo->join();
            
            CoStdOut::print("SubCo end %d\n", i);
            i++;
        }
    });
    engine.run();

    return 0;
}