#include "Coroutine.h"
#include "Engine.h"

//#include <memoryapi.h>

namespace OneCoroutine
{
    
    void Coroutine::coRun(void* s1, void* s2)
    {
        Coroutine* co = (Coroutine*)s1;
        //循环利用
        while (1)
        {
            co->run(s2);
        }
    }
    
    Coroutine::Coroutine(Engine* engine)
        :wakeupTimer(engine->timerManager)
        ,joinCond(engine), sleepCond(engine)
    {
        this->engine = engine;

        this->costack = (char*)malloc(COSTACK_SIZE);
        //this->costack = (char*)VirtualAlloc(nullptr, COSTACK_SIZE, MEM_RESERVE|MEM_WRITE_WATCH, 0);

        one_coctx_init(&coctx);
        one_coctx_make(&coctx, costack, COSTACK_SIZE, coRun, this, 0);
    }
    
    Coroutine::~Coroutine()
    {
        free(costack);
        //VirtualFree(costack, COSTACK_SIZE, MEM_RELEASE);
    }
        
    // Coroutine* Coroutine::getCurCoroutine()
    // {
    //     return Engine::getCurEngine()->getCurCoroutine();
    // }

    void Coroutine::run(void* data)
    {
        runner(this);

        //堆栈越界判断
        // if (costackBeginCheck != 0 || costackEndCheck != 0)
        // {
        //     //说明堆栈越界
        //     exit(-100);
        // }

        joinCond.active(true);              //激活所有等待
        engine->onCoRunExit(this);
    }
        
    void Coroutine::onDestruct()
    {
        engine->onCoDestruct(this);
    }
        
    void Coroutine::setRunner(const CoroutineRunner& runner)
    {
        this->runner = runner;
    }
      
    void Coroutine::startWakeupTimer(unsigned int delay)
    {
        wakeupTimer.start([this]() {
            engine->wakeup(this, Coroutine::SCHEDULE_ONTIME);
        }, delay, 0);
    }

    void Coroutine::stopWakeupTimer()
    {
        wakeupTimer.stop();
    }
        
    void Coroutine::join()
    {
        if (state == IDLE)
        {
            return;
        }
        if (engine->getMyCurCoroutine() == this)
        {
            //不能自己等自己
            return;
        }
        joinCond.wait();
    }
        
    void Coroutine::sleep(unsigned int msec)
    {
        if (msec > 0)
        {
            sleepCond.wait(msec);
        }
        else
        {
            engine->yield();
        }
    }
        
    void Coroutine::yield()
    {
        engine->yield();
    }
        
    // Engine* Coroutine::getEngine()
    // {
    //     return engine;
    // }
        
    char* Coroutine::getStack()
    {
        return costack;
    }

} // namespace One
