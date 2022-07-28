#include "CoTimer.h"
#include "Engine.h"

namespace OneCoroutine
{
    CoTimer::CoTimer()
        :_timer(&Engine::getCurEngine()->timerManager)
    {

    }

    void CoTimer::start(const TimerCB& cb, unsigned int delay, unsigned int interval)
    {      
        _cb = cb;  
        _timer.start([this]() {
            _coroutine = Engine::getCurEngine()->createCoroutine([this](Coroutine* co) {
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
    
} // namespace One
