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
        runner->__acquireObj__(true);       
        CoSystem::createCoroutine([runner](Coroutine* co) {
            runner->run();
            runner->__releaseObj__(true);
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
