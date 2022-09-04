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

    int Socket::listen(String* localAddr, int port, bool reuseaddr, int backlog)
    {
        return _sock->listen(localAddr->str(), port, reuseaddr, backlog);
    }
    int Socket::connect(String* addr, int port, int timeout)
    {
        return _sock->connect(addr->str(), port, timeout);
    }
    int Socket::accept(Socket* listenSocket, int timeout)
    {
        return _sock->accept(listenSocket->_sock, timeout);
    }
    void Socket::close()
    {
        _sock->close();
    }

    int Socket::send(Buffer* buffer, int timeout)
    {
        int ret = _sock->send(buffer->getData(), buffer->getDataLength(), timeout);
        if (ret > 0)
        {
            buffer->addReadPos(ret);
        }
        return ret;
    }
    int Socket::recv(Buffer* buffer, int timeout)
    {
        int ret = _sock->recv(buffer->getLeftBuf(), buffer->getLeftCapacity(), timeout);
        if (ret > 0)
        {
            buffer->addWritePos(ret);
        }
        return ret;
    }
}