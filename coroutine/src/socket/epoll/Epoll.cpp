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
        unsigned int time = SystemUtils::getMSTick();
        if (lastWaitTime == time)
        {
            //控制1ms调用一次
            if (timeout > 0)
            {
                SystamUtils::sleep(1);
            }
            return;
        }
        lastWaitTime = time;

        int eventNum = epoll_wait(epollFd, events, EVENT_NUM, timeout);
        for (int i = 0;i < eventNum;i++)
        {
            epoll_event& event = events[i];
            Socket* socket  = (Socket*)event.data.ptr;
            socket->onEvent(event.events & EPOLLIN, event.events & EPOLLOUT, 
                (event.events & EPOLLERR) || (event.events & EPOLLHUP));
        }
    }
        
    Engine* Epoll::getEngine()
    {
        return engine;
    }
    
} // namespace One

#endif