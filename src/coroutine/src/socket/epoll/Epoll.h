#pragma once
#ifndef _WIN32

#include "../Network.h"
#include "../../thread/Atomic.h"

namespace OneCoroutine
{
    class Engine;
    class Socket;
    class Epoll
    {
    public:
        Epoll(Engine* engine);
        ~Epoll();

    public:
        void registerEvent(Socket* socket, bool repeat);
        void unregisterEvent(Socket* socket);

        void wait(unsigned int timeout);
        void active();                                      //  激活等待

    protected:
        Engine* engine = nullptr;
        int epollFd = -1;

        //pipe用于激活wait
        AtomicInt waitState;
        int pipefd[2] = {-1, -1};
        
        unsigned int lastWaitTime = 0;
        static const int EVENT_NUM = 1000;
        epoll_event events[EVENT_NUM];
    };
} // namespace One

#endif
