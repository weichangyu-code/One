#ifndef _WIN32

#include "Epoll.h"
#include "../Network.h"
#include "Socket.h"
#include <signal.h>
#include "WatchTimer.h"
#include <stdio.h>
#include "SystemUtils.h"
using namespace OneCommon;

namespace OneCoroutine
{
    Epoll::Epoll(Engine* engine)
    {
        this->engine = engine;

	    signal(SIGPIPE, SIG_IGN);
        epollFd = epoll_create(1);

        pipe(pipefd);
        struct epoll_event ev;
        ev.data.ptr = nullptr;
        ev.events = EPOLLIN|EPOLLET;
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

        if (timeout > 0)
        {
            waitState.store(1);
        }
        int eventNum = epoll_wait(epollFd, events, EVENT_NUM, timeout);
        if (timeout > 0)
        {
            waitState.store(0);
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
                char buf[10];
                int i = ::read(pipefd[0], buf, sizeof(buf));
                // printf("aaaaaaaaaa read=%d\n", i);
            }
        }
    }
        
    void Epoll::active()
    {
        //if (waitState.load() == 1)
        {
            // struct epoll_event ev;
            // ev.data.ptr = nullptr;
            // ev.events = EPOLLOUT|EPOLLET;
            // epoll_ctl(epollFd, EPOLL_CTL_MOD, pipefd[1], &ev);
            ::write(pipefd[1], "", 1);

            // static int times = 0;
            // times++;
            // if (times % 100 == 0)
            // {
            //     printf("times=%d\n", times);
            // }
        }
    }
    
} // namespace One

#endif