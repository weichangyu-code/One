#include "CoCondition.h"
#include "Engine.h"

namespace OneCoroutine
{
    
    CoCondition::CoCondition()
        :CoCondition(Engine::getCurEngine())
    {

    }
        
    CoCondition::CoCondition(Engine* engine)
    {
        this->engine = engine;
    }

    bool CoCondition::wait(unsigned int timeout)
    {
        if (timeout > 0)
        {
            Coroutine* co = engine->getMyCurCoroutine();

            //不能在主线程等待，不然会崩溃。co=nullptr
            if (timeout != UINT_MAX)
            {
                co->startWakeupTimer(timeout);
                engine->onCoConditionWait(this);
                co->stopWakeupTimer();
                //判断是不是定时唤醒
                return (co->scheduleParam == Coroutine::SCHEDULE_CONDITION_ACTIVE);
            }
            else
            {
                engine->onCoConditionWait(this);
                return true;
            }

        }
        return false;
    }
    
    bool CoCondition::active(bool all)
    {
        return engine->onCoConditionActive(this, all);
    }
        
    bool CoCondition::haveWaiter()
    {
        return waitCos.empty() == false;
    }
    
} // namespace One
