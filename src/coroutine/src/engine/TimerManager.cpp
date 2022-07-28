#include "TimerManager.h"
#include "SystemUtils.h"
using namespace OneCommon;

namespace OneCoroutine
{
    TimerManager::TimerManager()
    {
        this->curTime = SystemUtils::getMSTick();
    }
    
    void TimerManager::startTimer(Timer* timer, unsigned int delay)
    {
        unsigned int activeTime = getSystemTime() + delay;
        unsigned int diff = activeTime - curTime;

        if (diff < 1024)
        {
            //小于1秒，放到毫秒池
            microSecArr[activeTime % 1024].pushTail(&timer->node);
        }
        else if (diff < 1024*1024)
        {
            secondArr[(activeTime / 1024) % 1024].pushTail(&timer->node);
        }
        else
        {
            timer->iter = timerMap.insert(TimerMap::value_type(activeTime, timer));
        }
        timer->activeTime = activeTime;
    }

    void TimerManager::stopTimer(Timer* timer)
    {
        timer->node.pop();
        if (timer->iter != timerMap.end())
        {
            timerMap.erase(timer->iter);
            timer->iter = timerMap.end();
        }
        timer->activeTime = 0;
    }

    void TimerManager::run()
    {
        unsigned int time = getSystemTime();

        //触发毫秒定时器
        unsigned int msStart = curTime % 1024;
        unsigned int msNum = time - curTime + 1;        //包含当前时间触发
        msNum = (msNum > 1024) ? 1024 : msNum;
        for (int i = 0;i < msNum;i++)
        {
            ListHead& head = microSecArr[(msStart + i) % 1024];
            if (head.empty() == false)
            {
                callOnTime(head);
            }
        }

        //触发秒定时器
        unsigned int secStart = (curTime / 1024) % 1024;
        unsigned int secNum = (time / 1024) - (curTime / 1024);
        secNum = (secNum > 1024) ? 1024 : secNum;
        for (int i = 0;i < secNum;i++)
        {
            ListHead& head = secondArr[(secStart + i) % 1024];
            if (head.empty() == false)
            {
                callOnTime(head);
            }
        }

        //当前秒的定时器转到毫秒
        if (secNum > 0)
        {
            roll(time);
        }

        curTime = time;
    }
        
    void TimerManager::callOnTime(ListHead& head)
    {
        while (head.empty() == false)
        {
            ListNode* node = head.head();
            node->pop();

            Timer* timer = GET_ENTRY(Timer, node, node);
            timer->onTime();
        }
    }
        
    void TimerManager::roll(unsigned int time)
    {
        ListHead& head = secondArr[(time / 1024) % 1024];
        if (head.empty() == false)
        {
            rollSecondArr(head, time);
        }

        //
        if (timerMap.empty() == false)
        {
            rollTimerMap(time);
        }
    }
        
    void TimerManager::rollSecondArr(ListHead& head, unsigned int time)
    {
        while (head.empty() == false)
        {
            ListNode* node = head.head();
            node->pop();

            Timer* timer = GET_ENTRY(Timer, node, node);
            if (UINT_COMPARE(timer->activeTime, time) <= 0)
            {
                //直接触发
                timer->onTime();
            }
            else
            {
                //放到毫秒盘
                microSecArr[timer->activeTime % 1024].pushTail(&timer->node);
            }
        }
    }
        
    void TimerManager::rollTimerMap(unsigned int time)
    {
        while (timerMap.empty() == false)
        {
            Timer* timer = timerMap.begin()->second;
            if (UINT_COMPARE(timer->activeTime, time) <= 0)
            {
                timerMap.erase(timer->iter);
                timer->iter = timerMap.end();

                //直接触发
                timer->onTime();
            }
            else if (UINT_COMPARE(timer->activeTime, time) < 1024*1024)
            {
                timerMap.erase(timer->iter);
                timer->iter = timerMap.end();

                secondArr[(timer->activeTime / 1024) % 1024].pushTail(&timer->node);
            }
            else
            {
                break;
            }
        }
    }
        
    unsigned int TimerManager::getSystemTime()
    {
        unsigned int time = SystemUtils::getMSTick();
        return (UINT_COMPARE(time, curTime) < 0) ? curTime : time;
    }

} // namespace One

