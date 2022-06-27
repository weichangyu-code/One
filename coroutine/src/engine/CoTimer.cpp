#include "CoTimer.h"
#include "Engine.h"

namespace OneCoroutine
{
    CoTimer::CoTimer()
        :CoTimer(Engine::getCurEngine())
    {

    }
        
    CoTimer::CoTimer(Engine* engine)
        :timer(engine->timerManager)
    {

    }

    void CoTimer::start(const TimerCB& cb, unsigned int delay, unsigned int interval)
    {
        timer.start([this, &cb]() {
            getEngine()->createCoroutine([&cb](Coroutine* co) {
                cb();
            });
        }, delay, interval);
    }
    void CoTimer::stop()
    {
        timer.stop();
    }
    bool CoTimer::isStart()
    {
        return timer.isStart();
    }
    Engine* CoTimer::getEngine()
    {
        return timer.getEngine();
    }
} // namespace One
