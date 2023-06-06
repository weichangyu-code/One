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
        OneInt listen(String* localAddr, OneInt port, OneBool reuseaddr = true, OneInt backlog = 128);
        OneInt connect(String* addr, OneInt port, OneInt timeout = -1);
        OneInt accept(Socket* listenSocket, OneInt timeout = -1);
        void close();

        OneInt send(Buffer* buffer, OneInt timeout = -1);
        OneInt recv(Buffer* buffer, OneInt timeout = -1);
        
    public:
        OneCoroutine::CoSocket* _sock = nullptr;
    };

}
