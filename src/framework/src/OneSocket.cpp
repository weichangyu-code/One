#include "OneSocket.h"
#include "engine/CoSocket.h"
using namespace OneCoroutine;

namespace One
{
    Socket::Socket()
    {
        _sock = new CoSocket();
    }
    Socket::~Socket()
    {
        delete _sock;
    }

    OneInt Socket::listen(String* localAddr, OneInt port, OneBool reuseaddr, OneInt backlog)
    {
        return _sock->listen((const char*)localAddr->str(), port, reuseaddr, backlog);
    }
    OneInt Socket::connect(String* addr, OneInt port, OneInt timeout)
    {
        return _sock->connect((const char*)addr->str(), port, timeout);
    }
    OneInt Socket::accept(Socket* listenSocket, OneInt timeout)
    {
        return _sock->accept(listenSocket->_sock, timeout);
    }
    void Socket::close()
    {
        _sock->close();
    }

    OneInt Socket::send(Buffer* buffer, OneInt timeout)
    {
        int ret = _sock->send((const char*)buffer->getData(), buffer->getDataLength(), timeout);
        if (ret > 0)
        {
            buffer->addReadPos(ret);
        }
        return ret;
    }
    OneInt Socket::recv(Buffer* buffer, OneInt timeout)
    {
        int ret = _sock->recv((char*)buffer->getLeftBuf(), buffer->getLeftCapacity(), timeout);
        if (ret > 0)
        {
            buffer->addWritePos(ret);
        }
        return ret;
    }
}