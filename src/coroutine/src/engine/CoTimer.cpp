#include "CoTimer.h"
#include "Engine.h"

namespace OneCoroutine
{
    CoTimer::CoTimer()
        :CoTimer(Engine::getCurEngine())
    {

    }
        
    CoTimer::CoTimer(Engine* engine)
        :_timer(engine->timerManager)
    {

    }

    void CoTimer::start(const TimerCB& cb, unsigned int delay, unsigned int interval)
    {      
        _cb = cb;  
        _timer.start([this]() {
            _coroutine = getEngine()->createCoroutine([this](Coroutine* co) {
                _cb();
            });
        }, delay, interval);
    }

    void CoTimer::stop()
    {
        if (_coroutine)
        {
            //还未执行的话，可以取消协程
            _coroutine->cancel();
            _coroutine.clear();
        }
        _timer.stop();
    }
    bool CoTimer::isStart()
    {
        return _timer.isStart();
    }
    Engine* CoTimer::getEngine()
    {
        return _timer.getEngine();
    }
} // namespace One
