#pragma once
#include "Common.h"
#include <mutex>
#include <condition_variable>
#include <list>
#include <thread>
#include <atomic>
#include "List.h"
#include "Event.h"
#include "LockFreeList.h"
#include "Semaphore.h"

namespace OneCoroutine
{
    class ThreadPool;

    /**
     * 新任务过来
     *  有空闲线程，获取空闲线程，并激活处理任务
     *  没空闲线程，如果现有线程数量小于最大值，新建线程，否则推入队列
     *  线程处理完成后，如果队列还有等待任务，取出一个，继续执行，否则等待1分钟
     *  1分钟后，还未被激活，当前线程数量小于最小线程数，继续等待，否则关闭线程
     */
    class Worker 
    {
    public:
        Worker(ThreadPool* pool);

    public:
        void execute(const SimpleFunction& func);

    public:
        ThreadPool* pool = nullptr;

        //当前要执行的任务
        SimpleFunction task;

        std::thread thd;
        volatile bool running = false;
        
        Semaphore sema;

        LockFreeNode node;
    };


    /**
     * 
     *  
     * 
     */
    class ThreadPool
    {
        friend class Worker;
    public:
        ThreadPool();

        void execute(const SimpleFunction& func);

    protected:
        void freeWorker(Worker* worker);
        Worker* getWorker();

        SimpleFunction popTask();
        void pushTask(const SimpleFunction& func);

        void wait(unsigned int msec);

    protected:
        unsigned int maxThreads = 100;
        unsigned int minThreads = 3;

        //Worker 对象管理，避免频繁创建销毁
        LockFreeStackHead freeWorkers;
        std::atomic_uint workerNum;

        //队列任务
        std::mutex mtxTasks;
        std::deque<SimpleFunction> tasks;
    };
}
