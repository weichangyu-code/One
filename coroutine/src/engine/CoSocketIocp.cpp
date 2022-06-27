#ifdef _WIN32

#include "CoSocket.h"
#include "Engine.h"
#include "CoEvent.h"
#include "Timer.h"

namespace OneCoroutine
{
    CoSocket::CoSocket()
        :CoSocket(Engine::getCurEngine())
    {

    }
    CoSocket::CoSocket(Engine* engine)
        :socket(engine->iocp)
    {

    }
    CoSocket::~CoSocket()
    {
        close();
    }

    int CoSocket::listen(const char* localAddr, int port, bool reuseaddr, int backlog)
    {
        int ret;
        //windows下要切到主线程，不然16K的堆栈不够用
        getEngine()->executeOnMain([=, &ret]() {
            ret = socket.listen(localAddr, port, reuseaddr, backlog);
        });
        return ret;
    }
        
    int CoSocket::connect(const char* addr, int port, unsigned int timeout)
    {
        //连接
        int ret;
        OperateOverlapped* oo;
        CoEvent event(getEngine());
        getEngine()->executeOnMain([=, &ret, &oo, &event]() {
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
        int ret;
        OperateOverlapped* oo;
        CoEvent event(getEngine());
        getEngine()->executeOnMain([=, &ret, &oo, &event]() {
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
        getEngine()->executeOnMain([this]() {
            socket.close();
        });
    }

    int CoSocket::send(const char* data, unsigned int len, unsigned int timeout)
    {
        int ret;
        OperateOverlapped* oo;
        CoEvent event(getEngine());
        getEngine()->executeOnMain([=, &ret, &oo, &event]() {
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
        int ret;
        OperateOverlapped* oo;
        CoEvent event(getEngine());
        getEngine()->executeOnMain([=, &ret, &oo, &event]() {
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
        getEngine()->executeOnMain([=]() {
            socket.setSendBuf(sendBuf);
        });
    }
        
    void CoSocket::setRecvBuf(unsigned int recvBuf)
    {
        getEngine()->executeOnMain([=]() {
            socket.setRecvBuf(recvBuf);
        });
    }

    Engine* CoSocket::getEngine()
    {
        return socket.getEngine();
    }
        
    void CoSocket::cancelIo(OperateOverlapped* oo)
    {
        getEngine()->executeOnMain([this, oo]() {
            socket.cancelIo(oo);
        });
    }
}

#endif