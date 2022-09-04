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
        int listen(String* localAddr, int port, bool reuseaddr, int backlog);
        int connect(String* addr, int port, int timeout);
        int accept(Socket* listenSocket, int timeout);
        void close();

        int send(Buffer* buffer, int timeout);
        int recv(Buffer* buffer, int timeout);
        
    public:
        OneCoroutine::CoSocket* _sock = nullptr;
    };

}
