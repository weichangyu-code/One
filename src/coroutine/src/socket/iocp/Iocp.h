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

        void registerEvent(HANDLE handle);
        void unregisterEvent(HANDLE handle);
        void cancelIo(OperateOverlapped* oo);
        
        void active();

        OperateOverlapped* mallocFromPool();
        void freeToPool(OperateOverlapped* oo);
        void clearOoPool();

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
