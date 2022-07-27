#pragma once

#ifdef _WIN32

#include "../Network.h"
#include "List.h"
using namespace OneCommon;

namespace OneCoroutine
{
    class Engine;
    class Socket;
    class OperateOverlapped;
    class Iocp
    {
    public:
        Iocp(Engine* engine);
        ~Iocp();

    public:
        void wait(unsigned int timeout);

        void registerEvent(Socket* socket);
        void unregisterEvent(Socket* socket);

        void cancelIo(Socket* socket, OperateOverlapped* oo);

        void post(OperateOverlapped* oo);

        OperateOverlapped* mallocFromPool();
        void freeToPool(OperateOverlapped* oo);
        void clearOoPool();

        Engine* getEngine();

    protected:
        bool dealwithOperateOverlapped(unsigned int timeout);
        int  getOperateOverlappedError(OperateOverlapped* oo);

    protected:
        Engine* engine;

		HANDLE cpHandle;

        static const int OO_POOL_SIZE = 1024;
        ListHead ooPool;
    };
} // namespace One

#endif
