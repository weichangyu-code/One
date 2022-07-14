#include "OneSystem.h"
#include "engine/CoSystem.h"
using namespace OneCoroutine;

namespace One
{
    void System::createCoroutine(Runner* runner)
    {
        if (runner == nullptr)
        {
            return;
        }
        runner->acquireObj(false);
        CoSystem::createCoroutine([runner](Coroutine* co) {
            runner->run();
            runner->releaseObj(false);
        });
    }
    
    void System::co(Runner* runner)
    {
        createCoroutine(runner);
    }
        
    void System::yield()
    {
        CoSystem::yield();
    }
} // namespace One
