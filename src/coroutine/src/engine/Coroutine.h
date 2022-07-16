#pragma once
#include "../coctx/coctx.h"
#include "List.h"
#include "CoCondition.h"
#include "Timer.h"
#include "AutoPtr.h"

namespace OneCoroutine
{
    class Coroutine;
    typedef std::function<void(Coroutine* co)> CoroutineRunner;
    typedef std::function<void()> AsyncFunction;

    class Engine;
    class Coroutine : public AutoObject
    {
        friend class Engine;
        friend class CoCondition;
        Coroutine(Engine* engine);
        ~Coroutine();
    public:   
        typedef AutoPtr<Coroutine> Ptr;

    public:
        //static Coroutine* getCurCoroutine();
        inline Engine* getEngine()
        {
            return engine;
        }

        void setRunner(const CoroutineRunner& runner);

        char* getStack();

        void join();
        void sleep(unsigned int msec);
        void yield();

    protected:
        static void coRun(void* s1, void* s2);
        void run(void* data);
        virtual void onDestruct();

        void startWakeupTimer(unsigned int delay);
        void stopWakeupTimer();

    protected:
        Engine* engine = nullptr;
        CoroutineRunner runner;

        //调度引擎管理
        enum
        {
            IDLE = 0,
            READY,      //在等待队列
            WAIT,
            RUN,
        };
        int state = IDLE;
        ListNode scheduleNode;          //调度节点

        enum
        {
            SCHEDULE_NORMAL = 0,        //普通轮询
            SCHEDULE_CONDITION_ACTIVE,  //事件激活
            SCHEDULE_ONTIME,            //定时器触发
        };
        int scheduleParam = 0;          //调度参数

        //
        ListNode lifeNode;

        //关闭等待
        CoCondition joinCond;

        //睡眠
        CoCondition sleepCond;

        //唤醒定时器
        Timer wakeupTimer;

        //
        coctx_t coctx;
        const unsigned int COSTACK_SIZE = 16*1024;
        char* costack;            //8K会导致prinf的崩溃，后续等所有IO都到主线程后，在调整大小
    };
} // namespace One

