#pragma once
#include "OneObject.h"
#include "OneString.h"
#include "OneBuffer.h"

namespace OneCoroutine
{
    class CoSocket;
}

namespace One
{
    class Socket : public Object
    {
    public:
        Socket();
        ~Socket();

    public:
        int listen(String* localAddr, int port, bool reuseaddr = true, int backlog = 128);
        int connect(String* addr, int port, int timeout = -1);
        int accept(Socket* listenSocket, int timeout = -1);
        void close();

        int send(Buffer* buffer, int timeout = -1);
        int recv(Buffer* buffer, int timeout = -1);
        
    public:
        OneCoroutine::CoSocket* _sock = nullptr;
    };

}
