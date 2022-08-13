#include "ThreadPool.h"

namespace OneCoroutine
{
    Worker::Worker(ThreadPool* pool)
    {
        this->pool = pool;
        printf("new work\n");
    }
        
    void Worker::execute(const SimpleFunction& func)
    {
        if (running == false)
        {
            running = true;
            task = func;
            thd = std::thread([this]() {
                do
                {
                    task();
                    task = pool->popTask();
                    if (task)
                    {
                        continue;
                    }
                    
                    pool->freeWorker(this);
                    sema.wait();
                } while (running);
            });
        }
        else
        {
            //必须加锁，确保线程已经在等待
            task = func;
            sema.post();
        }
    }


    ThreadPool::ThreadPool()
        :workerNum(0)
    {
        //threads.size
    }

    void ThreadPool::freeWorker(Worker* worker)
    {
        freeWorkers.push(&worker->node);
    }
        
    Worker* ThreadPool::getWorker()
    {
        Worker* worker = nullptr;

        LockFreeNode* node = freeWorkers.pop();
        if (node == nullptr)
        {
            //判断和+1不是原子操作，可能会导致一定的误差
            if (workerNum.load() >= maxThreads)
            {
                return nullptr;
            }
            workerNum.fetch_add(1);
            worker = new Worker(this);
        }
        else
        {
            worker = GET_ENTRY(Worker, node, node);
        }
        return worker;
    }
        
    SimpleFunction ThreadPool::popTask()
    {
        if (tasks.empty())
        {
            return {};
        }

        std::lock_guard<std::mutex> l(mtxTasks);
        if (tasks.empty() == false)
        {
            SimpleFunction func = tasks.front();
            tasks.pop_front();
            return func;
        }
        else
        {
            return {};
        }
    }
        
    void ThreadPool::pushTask(const SimpleFunction& func)
    {
        std::lock_guard<std::mutex> l(mtxTasks);
        tasks.push_back(func);
    }
    
    void ThreadPool::execute(const SimpleFunction& func)
    {
        //有空闲线程，直接触发
        //没空闲线程，新建线程
        //线程数达到最大后，进入队列等待
        Worker* worker = getWorker();
        if (worker)
        {
            worker->execute(func);
        }
        else
        {
            pushTask(func);
        }
    }
}


