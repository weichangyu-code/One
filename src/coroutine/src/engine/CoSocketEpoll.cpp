
#ifndef _WIN32

#include "CoSocket.h"
#include "Engine.h"
#include "CoEvent.h"
#include "Timer.h"
#include "../socket/Network.h"
#include "CoStdOut.h"

namespace OneCoroutine
{
    CoSocket::CoSocket()
        : socket(Engine::getCurEngine()->epoll, this)
        , readEvent(false)
    {

    }
    CoSocket::~CoSocket()
    {
        close();
    }

    int CoSocket::listen(const char* localAddr, int port, bool reuseaddr, int backlog)
    {
        return socket.listen(localAddr, port, reuseaddr, backlog);
    }
        
    int CoSocket::connect(const char* addr, int port, unsigned int timeout)
    {
        //连接
        int ret = socket.connect(addr, port);
        if (ret != ERR_SOCKET_INPROGRESS)
        {
            return ret;
        }
        if (connectEvent.wait(timeout) == false)
        {
            close();
            return ERR_SOCKET_TIMEOUT;
        }

        //等待成功
        if (exceptEvent.isSignal())
        {
            //失败
            close();
            return ERR_ERROR;
        }

        return ERR_SUCCESS;
    }
        
    int CoSocket::accept(CoSocket* listenSocket, unsigned int timeout)
    {
        int ret;
        while (1)
        {
            ret = socket.accept(&listenSocket->socket);
            if (ret != ERR_SOCKET_AGAIN)
            {
                return ret;
            }
            listenSocket->readEvent.signal(false);
            if (listenSocket->readEvent.wait(timeout) == false)
            {
                return ERR_SOCKET_TIMEOUT;
            }
        }
        return ret;
    }
        
    void CoSocket::close()
    {
        writeEvent.signal(false);
        readEvent.signal(false);
        exceptEvent.signal(false);
        connectEvent.signal(false);
        socket.close();
    }

    int CoSocket::send(const char* data, unsigned int len, unsigned int timeout)
    {
        int sendBytes = 0;
        while (len > 0)
        {
            int ret = socket.send(data, len);
            if (ret > 0)
            {
                sendBytes += ret;
                data += ret;
                len -= ret;
                continue;
            }
            else if (ret == 0)
            {
                break;
            }
            else if (ret == ERR_SOCKET_AGAIN)
            {
                if (writeEvent.wait(timeout) == false)
                {
                    //超时跳出
                    break;
                }
            }
            else
            {
                //错误
                return ret;
            }
        }
        return sendBytes;
    }
        
    int CoSocket::recv(char* data, unsigned int len, unsigned int timeout)
    {
        int ret;
        while (1)
        {
            if (readEvent.wait(timeout) == false)
            {
                return ERR_SOCKET_TIMEOUT;
            }
            ret = socket.recv(data, len);
            if (ret != len) readEvent.signal(false);
            if (ret != ERR_SOCKET_AGAIN)
            {
                return ret;
            }
        }
        return ret;
    }
        
    void CoSocket::setSendBuf(unsigned int sendBuf)
    {
        socket.setSendBuf(sendBuf);
    }
        
    void CoSocket::setRecvBuf(unsigned int recvBuf)
    {
        socket.setRecvBuf(recvBuf);
    }
    
    void CoSocket::onEvent(bool read, bool write, bool exception)
    {
        if (write) writeEvent.signal(true);
        if (read) readEvent.signal(true);
        if (exception) exceptEvent.signal(true);
        if (connectEvent.haveWaiter()) connectEvent.signal(true);
    }
}

#endif
