#include "CoSystem.h"
#include "Engine.h"

namespace OneCoroutine
{
    void CoSystem::sleep(unsigned int msec)
    {
        Engine::getCurCoroutine()->sleep(msec);
    }
    
    void CoSystem::yield()
    {
        Engine::getCurEngine()->yield();
    }
    
    void CoSystem::executeOnMain(const AsyncFunction& func)
    {
        Engine::getCurEngine()->executeOnMain(func);
    }
    
    Coroutine::Ptr CoSystem::createCoroutine(const CoroutineRunner& runner)
    {
        return Engine::getCurEngine()->createCoroutine(runner);
    }

} // namespace One