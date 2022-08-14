#ifndef _WIN32

#include "Epoll.h"
#include "../Network.h"
#include "Socket.h"
#include <signal.h>
#include "WatchTimer.h"
#include <stdio.h>
#include "SystemUtils.h"
#include <thread>
using namespace OneCommon;

namespace OneCoroutine
{
    Epoll::Epoll(Engine* engine)
    {
        this->engine = engine;

	    signal(SIGPIPE, SIG_IGN);

        epollFd = epoll_create(1);

        //添加管道，用于队列激活
        pipe(pipefd);
        struct epoll_event ev;
        ev.data.ptr = nullptr;
        ev.events = EPOLLIN;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, pipefd[0], &ev);
    }

    Epoll::~Epoll()
    {
        close(epollFd);
    }

    void Epoll::registerEvent(Socket* socket, bool repeat)
    {
        struct epoll_event ev;
        ev.data.ptr = socket;
        ev.events = EPOLLIN|EPOLLOUT;
        if (repeat == false)
        {
            ev.events |= EPOLLET;
        }

        epoll_ctl(epollFd, EPOLL_CTL_ADD, socket->sockFd, &ev);
    }

    void Epoll::unregisterEvent(Socket* socket)
    {
        struct epoll_event ev;
        ev.data.ptr = socket;
        ev.events = 0;
        epoll_ctl(epollFd, EPOLL_CTL_DEL, socket->sockFd, &ev);
    }

    void Epoll::wait(unsigned int timeout)
    {
        //控制调用频率，同1ms只调用一次
        //这个在损失1ms响应的情况下，大大的提高性能。暂时屏蔽，后期通过Semaphore::wait替代，因为可以中间激活
        // unsigned int time = SystemUtils::getMSTick();
        // if (lastWaitTime == time)
        // {
        //     //控制1ms调用一次
        //     if (timeout > 0)
        //     {
        //         SystemUtils::sleep(1);
        //     }
        //     return;
        // }
        // lastWaitTime = time;

        int eventNum = 0;
        if (timeout > 0)
        {
            waitState.store(1);
            eventNum = epoll_wait(epollFd, events, EVENT_NUM, timeout);
            waitState.store(0);
        }
        else
        {
            eventNum = epoll_wait(epollFd, events, EVENT_NUM, 0);
        }

        for (int i = 0;i < eventNum;i++)
        {
            epoll_event& event = events[i];
            Socket* socket  = (Socket*)event.data.ptr;
            if (socket)
            {
                socket->onEvent(event.events & EPOLLIN, event.events & EPOLLOUT, 
                    (event.events & EPOLLERR) || (event.events & EPOLLHUP));
            }
            else
            {
                char buf[32];
                ::read(pipefd[0], buf, sizeof(buf));
            }
        }
    }
        
    void Epoll::active()
    {
        if (waitState.load() == 1)
        {
            ::write(pipefd[1], "", 1);
        }
    }
    
} // namespace One

#endif