#ifdef _WIN32

#include "CoSocket.h"
#include "Engine.h"
#include "CoEvent.h"
#include "Timer.h"

namespace OneCoroutine
{
    CoSocket::CoSocket()
        :socket(Engine::getCurEngine()->iocp)
    {

    }
    CoSocket::~CoSocket()
    {
        close();
    }

    int CoSocket::listen(const char* localAddr, int port, bool reuseaddr, int backlog)
    {
        int ret = ERR_SUCCESS;
        //windows下要切到主线程，不然16K的堆栈不够用
        Engine::getCurEngine()->executeOnMain([=, &ret]() {
            ret = socket.listen(localAddr, port, reuseaddr, backlog);
        });
        return ret;
    }
        
    int CoSocket::connect(const char* addr, int port, unsigned int timeout)
    {
        //连接
        int ret = ERR_SUCCESS;
        OperateOverlapped* oo = nullptr;
        CoEvent event;
        Engine::getCurEngine()->executeOnMain([=, &ret, &oo, &event]() {
            socket.connect(addr, port, &oo, [&ret, &event](OperateOverlapped* oo) {
                ret = oo->error;
                event.signal();
            });
        });
        if (event.wait(timeout) == false)
        {
            //取消io
            cancelIo(oo);

            //等待取消完成
            event.wait();
        }
        return ret;
    }
        
    int CoSocket::accept(CoSocket* listenSocket, unsigned int timeout)
    {
        int ret = ERR_SUCCESS;
        OperateOverlapped* oo = nullptr;
        CoEvent event;
        Engine::getCurEngine()->executeOnMain([=, &ret, &oo, &event]() {
            socket.accept(&listenSocket->socket, &oo, [&ret, &event](OperateOverlapped* oo) {
                ret = oo->error;
                event.signal();
            });
        });
        if (event.wait(timeout) == false)
        {
            //取消io
            cancelIo(oo);

            //等待取消完成
            event.wait();
        }
        return ret;
    }
        
    void CoSocket::close()
    {
        Engine::getCurEngine()->executeOnMain([this]() {
            socket.close();
        });
    }

    int CoSocket::send(const char* data, unsigned int len, unsigned int timeout)
    {
        int ret = ERR_SUCCESS;
        OperateOverlapped* oo = nullptr;
        CoEvent event;
        Engine::getCurEngine()->executeOnMain([=, &ret, &oo, &event]() {
            socket.send(data, len, &oo, [&ret, &event](OperateOverlapped* oo) {
                if (oo->error == 0)
                {
                    ret = oo->trans;
                }
                else
                {
                    ret = oo->error;
                }
                event.signal();
            });
        });
        if (event.wait(timeout) == false)
        {
            //取消io
            cancelIo(oo);

            //等待取消完成
            event.wait();
        }
        return ret;
    }
        
    int CoSocket::recv(char* data, unsigned int len, unsigned int timeout)
    {
        int ret = ERR_SUCCESS;
        OperateOverlapped* oo = nullptr;
        CoEvent event;
        Engine::getCurEngine()->executeOnMain([=, &ret, &oo, &event]() {
            socket.recv(data, len, &oo, [&ret, &event](OperateOverlapped* oo) {
                if (oo->error == 0)
                {
                    ret = oo->trans;
                }
                else
                {
                    ret = oo->error;
                }
                event.signal();
            });
        });
        if (event.wait(timeout) == false)
        {
            //取消io
            cancelIo(oo);

            //等待取消完成
            event.wait();
        }
        return ret;
    }
        
    void CoSocket::setSendBuf(unsigned int sendBuf)
    {
        Engine::getCurEngine()->executeOnMain([=]() {
            socket.setSendBuf(sendBuf);
        });
    }
        
    void CoSocket::setRecvBuf(unsigned int recvBuf)
    {
        Engine::getCurEngine()->executeOnMain([=]() {
            socket.setRecvBuf(recvBuf);
        });
    }
        
    void CoSocket::cancelIo(OperateOverlapped* oo)
    {
        Engine::getCurEngine()->executeOnMain([this, oo]() {
            socket.cancelIo(oo);
        });
    }
}

#endif