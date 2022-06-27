#include "OneSystem.h"
#include "engine/CoSystem.h"
using namespace OneCoroutine;

namespace One
{
    void System::createCoroutine(CoRunner* runner)
    {
        runner->acquireObj(false);
        CoSystem::createCoroutine([runner](Coroutine* co) {
            runner->run();
            runner->releaseObj(false);
        });
    }
    
    void System::co(CoRunner* runner)
    {
        createCoroutine(runner);
    }
        
    void System::yield()
    {
        CoSystem::yield();
    }
} // namespace One
