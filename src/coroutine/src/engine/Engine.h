#pragma once
#include "../coctx/coctx.h"
#include "List.h"
#include "TimerManager.h"
#include <functional>
#include "Coroutine.h"
#include "../thread/AsyncQueue.h"
#include "../thread/ThreadPool.h"

namespace OneCoroutine
{
    //使用r15保持curEngine，编译的时候通过-ffixed-r15，禁用r15
    class Engine;
    register Engine* curEngine asm ("r15");

    class Iocp;
    class Epoll;
    class CoCondition;
    class Engine
    {
        friend class CoCondition;
        friend class Coroutine;
        friend class CoSocket;
    public:
        Engine();
        ~Engine();

        //获取当前线程下的引擎
        inline static Engine* getCurEngine()
        {
            return curEngine;
        }
        inline static Coroutine* getCurCoroutine()
        {
            return curEngine->curCo;
        }
        inline Coroutine* getMyCurCoroutine()
        {
            return curCo;
        }

        //创建协程
        Coroutine::Ptr createCoroutine(const CoroutineRunner& runner);

        //所有协程都退出的时候，run退出。
        void run();

        //
        void yield();

        //切到主线程执行，和协程在同一个线程里
        void executeOnMain(const SimpleFunction& func);

        //切到线程池执行，跨线程，要注意数据包含，不要涉及可变数据
        void executeOnPool(const SimpleFunction& func);

    protected:
        void onCoRunExit(Coroutine* co);
        void onCoDestruct(Coroutine* co);
        void onCoConditionWait(CoCondition* cond);
        bool onCoConditionActive(CoCondition* cond, bool all);
        void onCoCancel(Coroutine* co);

        void scheduleOnCoRun();
        void scheduleOnCoNoRun();
        void scheduleOnMain();
        void scheduleToMain();
        void scheduleToMainOnCoRun();
        void wakeup(Coroutine* co, int param);

        void pushToScheduleFront(Coroutine* co);
        void pushToSchedule(Coroutine* co, bool first, int param);
        Coroutine* popFromSchedule(int state);

        void freeToPool(Coroutine* co);
        Coroutine* mallocFromPool();
        void clearPool();


    public:
        //定时器
        TimerManager timerManager;

        //异步消息
        AsyncQueue asyncQueue;

        //网络
#ifdef _WIN32
        Iocp* iocp = nullptr;
#else
        Epoll* epoll = nullptr;
#endif

        ThreadPool threadPool;

    protected:
        //调度相关
        coctx_t mainCoctx;
        static const unsigned int SCHEDULE_COUNT_TO_MAIN = 1024;
        int scheduleCountDown = SCHEDULE_COUNT_TO_MAIN;
        Coroutine* curCo = nullptr;
        ListHead scheduleList;

        //所有活着的
        ListHead lifeList;

        //
        SimpleFunction funcOnMain;

        //协程池
        static const int MAX_POOL_SIZE = 100;
        ListHead poolList;
    };
} // namespace One
