
#ifdef _WIN32

#define _WIN32_WINNT 0x0600

#include "Iocp.h"
#include "Socket.h"
#include <assert.h>
#include "../../engine/Engine.h"

namespace OneCoroutine
{
    Iocp::Iocp(Engine* engine)
    {
        this->engine = engine;

        WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		WSAStartup(wVersionRequested, &wsaData);

		cpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
    }

    Iocp::~Iocp()
    {
		::CloseHandle(cpHandle);
    }
        
    void Iocp::wait(unsigned int timeout)
    {
        static const int NUM_ONE_TIME = 1000;
        int num = 0;

        //清空队列
        while (num < NUM_ONE_TIME)
        {
            if (dealwithOperateOverlapped(timeout))
            {
                timeout = 0;
                num++;
            }
            else
            {
                break;
            }
        }
    }
        
    bool Iocp::dealwithOperateOverlapped(unsigned int timeout)
    {
        ULONG_PTR tmp = 0;
        DWORD trans = 0;
        OperateOverlapped* oo = NULL;
        BOOL ret = GetQueuedCompletionStatus(cpHandle, &trans, (PULONG_PTR)&tmp, (LPOVERLAPPED*)&oo, timeout);
        if (oo)
        {
            if (ret)
            {
                oo->trans = trans;
                oo->error = 0;
            }
            else
            {
                //如果发送对方没有反应，会发送RST包并关闭
                int err = getOperateOverlappedError(oo);
                oo->error = SOCKET_ERR_ERROR;
            }
            if (oo->socket)
            {
                oo->socket->onEvent(oo);
            }
            else
            {
                oo->cb(oo);
            }
            return true;
        }
        return false;
    }
        
    int Iocp::getOperateOverlappedError(OperateOverlapped* oo)
    {
        DWORD dwTrans;  
        DWORD dwFlags;
        int err = 0;
        if(FALSE == WSAGetOverlappedResult(oo->socket->sockFd, &oo->ol, &dwTrans, FALSE, &dwFlags))
        {
            err = WSAGetLastError();
        }
        return err;
    }

    void Iocp::registerEvent(Socket* socket)
    {
		HANDLE handle = ::CreateIoCompletionPort((HANDLE)(intptr_t)socket->sockFd, cpHandle, (ULONG_PTR)0, 0);
        assert(handle != 0);
    }
        
    void Iocp::unregisterEvent(Socket* socket)
    {
        ::CancelIoEx((HANDLE)(intptr_t)socket->sockFd, nullptr);
    }
        
    void Iocp::cancelIo(Socket* socket, OperateOverlapped* oo)
    {
        ::CancelIoEx((HANDLE)(intptr_t)socket->sockFd, &oo->ol);
    }
        
    void Iocp::post(OperateOverlapped* oo)
    {
        ::PostQueuedCompletionStatus(cpHandle, 0, 0, &oo->ol);
    }
        
    OperateOverlapped* Iocp::mallocFromPool()
    {
        OperateOverlapped* oo;
        if (ooPool.empty() == false)
        {
            ListNode* node = ooPool.head();
            node->pop();

            oo = GET_ENTRY(OperateOverlapped, poolNode, node);
        }
        else
        {
            oo = new OperateOverlapped();
        }
        oo->socket = nullptr;
        memset(&oo->ol, 0, sizeof(oo->ol));
        oo->trans = 0;
        oo->error = 0;
        return oo;
    }
        
    void Iocp::freeToPool(OperateOverlapped* oo)
    {
        if (ooPool.size() < OO_POOL_SIZE)
        {
            ooPool.pushHead(&oo->poolNode);
        }
        else
        {
            delete oo;
        }
    }
        
    void Iocp::clearOoPool()
    {
        while (ooPool.empty() == false)
        {
            ListNode* node = ooPool.tail();
            node->pop();

            delete GET_ENTRY(OperateOverlapped, poolNode, node);
        }
    }
        
    Engine* Iocp::getEngine()
    {
        return engine;
    }

    
} // namespace One

#endif
