#include "Engine.h"
#include <assert.h>
#include "Coroutine.h"
#include <algorithm>
#include "CoCondition.h"
#include "SystemUtils.h"
#include "../socket/epoll/Epoll.h"
#include "../socket/iocp/Iocp.h"

namespace OneCoroutine
{
    thread_local Engine* curEngine;

    Engine::Engine()
    {
        one_coctx_init(&mainCoctx);

        curEngine = this;

        timerManager = new TimerManager(this);

#ifdef _WIN32
        iocp = new Iocp(this);
#else
        epoll = new Epoll(this);
#endif
    }
        
    Engine::~Engine()
    {
        clearPool();

        delete timerManager;

#ifdef _WIN32
        delete iocp;
#else
        delete epoll;
#endif
    }
    
    Engine* Engine::getCurEngine()
    {
        return curEngine;
    }

    Coroutine* Engine::getCurCoroutine()
    {
        return curCo;
    }
        
    void Engine::yield()
    {
        scheduleOnCoRun();
    }
        
    void Engine::run()
    {
        curEngine = this;
        while (lifeList.empty() == false)
        {
            scheduleOnMain();

            //没调度程序，休眠1ms
#ifdef _WIN32
            iocp->wait(scheduleList.empty() ? 10 : 0);
#else
            epoll->wait(scheduleList.empty() ? 10 : 0);
#endif

            //定时器在主线程跑的，不能在定时器回调里面切换协程
            timerManager->run();
        }
    }

    void Engine::onCoRunExit(Coroutine* co)
    {
        assert(co == curCo);
        
        //协程结束，需要切回主线程
        co->state = Coroutine::IDLE;

        //移到删除队列
        lifeList.pop(&co->lifeNode);
        co->release();

        scheduleOnCoNoRun();
    }
        
    void Engine::onCoDestruct(Coroutine* co)
    {
        freeToPool(co);
    }
        
    void Engine::onCoConditionWait(CoCondition* cond)
    {
        curCo->state = Coroutine::WAIT;
        cond->waitCos.pushTail(&curCo->scheduleNode);

        scheduleOnCoNoRun();
    }

    bool Engine::onCoConditionActive(CoCondition* cond, bool all)
    {
        if (all)
        {
            bool ret = cond->waitCos.empty() == false;
            while (cond->waitCos.empty() == false)
            {
                ListNode* node = cond->waitCos.head();
                node->pop();

                Coroutine* co = GET_ENTRY(Coroutine, scheduleNode, node);
                wakeup(co, Coroutine::SCHEDULE_CONDITION_ACTIVE);
            }
            return ret;
        }
        else
        {
            if (cond->waitCos.empty() == false)
            {
                ListNode* node = cond->waitCos.head();
                node->pop();

                Coroutine* co = GET_ENTRY(Coroutine, scheduleNode, node);
                wakeup(co, Coroutine::SCHEDULE_CONDITION_ACTIVE);

                return true;
            }
        }
        return false;
    }

    Coroutine::Ptr Engine::createCoroutine(const CoroutineRunner& runner)
    {
        Coroutine* co = mallocFromPool();
        co->setRunner(runner);

        lifeList.pushTail(&co->lifeNode);
        pushToSchedule(co, 0);

        return co;
    }
        
    void Engine::scheduleOnCoRun()
    {
        if (--scheduleCountDown <= 0)
        {
            scheduleToMain();
            return;
        }
        
        register coctx_t* from = &curCo->coctx;
        pushToSchedule(curCo, 0);

        //取出下一个要执行的协程
        curCo = popFromSchedule(Coroutine::RUN);
        register coctx_t* to = &curCo->coctx;

        if (from != to)
        {
            one_coctx_swap(from, to);
        }
    }
        
    void Engine::scheduleOnCoNoRun()
    {
        if (--scheduleCountDown <= 0)
        {
            scheduleToMain();
            return;
        }
        
        register coctx_t* from = &curCo->coctx;

        //取出下一个要执行的协程
        register coctx_t* to = &mainCoctx;
        curCo = popFromSchedule(Coroutine::RUN);
        if (curCo)
        {
            to = &curCo->coctx;
        }

        one_coctx_swap(from, to);
    }
        
    void Engine::scheduleToMain()
    {
        if (curCo)
        {
            register coctx_t* from = &curCo->coctx;
            if (curCo->state == Coroutine::RUN)
            {
                pushToSchedule(curCo, 0);
            }
            curCo = nullptr;

            one_coctx_swap(from, &mainCoctx);
        }
    }
    
    void Engine::scheduleToMainOnCoRun()
    {
        register coctx_t* from = &curCo->coctx;
        pushToScheduleFront(curCo);
        curCo = nullptr;

        one_coctx_swap(from, &mainCoctx);
    }
        
    void Engine::scheduleOnMain()
    {
    repeat:
        curCo = popFromSchedule(Coroutine::RUN);
        if (curCo)
        {
            one_coctx_swap(&mainCoctx, &curCo->coctx);

            if (funcOnMain)
            {
                funcOnMain();
                funcOnMain = nullptr;
                goto repeat;
            }
        }
        scheduleCountDown = SCHEDULE_COUNT_TO_MAIN;
    }
        
    void Engine::wakeup(Coroutine* co, int param)
    {
        if (co->state != Coroutine::WAIT)
        {
            return;
        }

        co->scheduleNode.pop();
        pushToSchedule(co, param);
    }
        
    void Engine::executeOnMain(const AsyncFunction& func)
    {
        if (curCo)
        {
            funcOnMain = func;
            scheduleToMainOnCoRun();
        }
        else
        {
            func();
        }
    }
        
    void Engine::pushToScheduleFront(Coroutine* co)
    {
        co->state = Coroutine::READY;
        scheduleList.pushHead(&co->scheduleNode);
    }

    void Engine::pushToSchedule(Coroutine* co, int param)
    {
        co->scheduleParam = param;
        co->state = Coroutine::READY;
        scheduleList.pushTail(&co->scheduleNode);
    }

    Coroutine* Engine::popFromSchedule(int state)
    {
        if (scheduleList.empty() == false)
        {
            ListNode* node = scheduleList.head();
            node->pop();

            Coroutine* co = GET_ENTRY(Coroutine, scheduleNode, node);
            co->state = state;
            return co;
        }
        return nullptr;
    }
        
    void Engine::freeToPool(Coroutine* co)
    {
        poolList.pushHead(&co->lifeNode);

        //确保MAX_POOL_SIZE大于1，co不会被立刻销毁。因为还要保存coctx
        assert(MAX_POOL_SIZE > 1);
        if (poolList.size() > MAX_POOL_SIZE)
        {
            //删除尾部
            ListNode* node = poolList.tail();
            node->pop();

            delete GET_ENTRY(Coroutine, lifeNode, node);
        }
    }
        
    Coroutine* Engine::mallocFromPool()
    {
        if (poolList.empty() == false)
        {
            ListNode* node = poolList.head();
            node->pop();

            Coroutine* co = GET_ENTRY(Coroutine, lifeNode, node);
            co->resetRefCount();
            return co;
        }
        else
        {
            return new Coroutine(this);
        }
    }
    
    void Engine::clearPool()
    {
        while (poolList.empty() == false)
        {
            ListNode* node = poolList.tail();
            node->pop();

            delete GET_ENTRY(Coroutine, lifeNode, node);
        }
    }

} // namespace One

