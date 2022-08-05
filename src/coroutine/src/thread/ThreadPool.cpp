#include "ThreadPool.h"

namespace OneCoroutine
{
    Worker::Worker(ThreadPool* pool)
    {
        this->pool = pool;
    }
        
    bool Worker::execute(const SimpleFunction& func)
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
                    
                    unique_lock<mutex> l(mtx);
                    pool->freeWorker(this);
                    cond.wait(l);
                } while (running);
            });
        }
        else
        {
            //必须加锁，确保线程已经在等待
            if (mtx.try_lock() == false)
            {
                //锁冲突了，放弃这个WORKER
                return false;
            }
            task = func;
            cond.notify_one();
            mtx.unlock();
        }
        return true;
    }


    ThreadPool::ThreadPool()
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
        LockFreeStackHead head;

        //有空闲线程，直接触发
        //没空闲线程，新建线程
        //线程数达到最大后，进入队列等待
        Worker* worker = getWorker();
        while (worker)
        {
            if (worker->execute(func))
            {
                break;
            }

            head.pushUnsafe(&worker->node);
            worker = getWorker();
        }

        for (LockFreeNode* next = head.head();next != nullptr;next = next->next())
        {
            freeWorker(GET_ENTRY(Worker, node, next));
        }

        //最后没成功，还是放到队列
        if (worker == nullptr)
        {
            pushTask(func);
        }
    }
}


