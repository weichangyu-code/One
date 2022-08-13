#include "ThreadPool2.h"
#include <thread>
#include "SystemUtils.h"
using namespace OneCommon;

namespace OneCoroutine
{
    ThreadPool2::ThreadPool2()
        :queue(4096)
    {
    }

    void ThreadPool2::execute(const SimpleFunction& func)
    {
        this->queueNum.exchange_add(1);
        queue.push(func);
        sema.post();

        int threadNum = (int)this->threadNum.get();
        int queueNum = (int)this->queueNum.get();
        if (threadNum < queueNum && threadNum < maxThreads)
        {
            //新建线程
            createThread(threadNum < minThreads);
        }
    }
        
    void ThreadPool2::createThread(bool core)
    {
        threadNum.exchange_add(1);
        thread thd = thread([this, core]() {

            printf("create thread\n");

            while (1)
            {
                if (sema.wait(core ? UINT_MAX : threadTimeout / 3))
                {
                    SimpleFunction func;
                    if (queue.pop(func))
                    {
                        func();
                        queueNum.exchange_add(-1);
                    }
                }

                //判断线程是否需要关闭
                if (shouldExitThread())
                {
                    if (core == false)
                    {
                        //非核心的才会关闭
                        break;
                    }
                }
            }
            threadNum.exchange_add(-1);
            
            printf("exit thread\n");
        });
        thd.detach();
    }
        
    bool ThreadPool2::shouldExitThread()
    {
        unsigned int curTime = SystemUtils::getMSTick();

        int threadNum = (int)this->threadNum.get();
        int queueNum = (int)this->queueNum.get();
        if (queueNum + 2 > threadNum)
        {
            //队列数量超过线程数量，代表比较忙
            busyTime = curTime;
            return false;
        }
        
        return UINT_COMPARE(busyTime + threadTimeout, curTime) < 0;
    }
    
} // namespace OneCoroutine
