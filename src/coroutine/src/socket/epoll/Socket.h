#pragma once

#ifndef _WIN32

namespace OneCoroutine
{
    class SocketListener
    {
    public:
        virtual void onEvent(bool read, bool write, bool exception) {};
    };

    class Epoll;
    class Engine;
    class Socket
    {
        friend class Epoll;
    public:
        Socket(Epoll* epoll, SocketListener* listener);
        ~Socket();

        int listen(const char* localAddr, int port, bool reuseaddr, int backlog);
        int connect(const char* addr, int port);
        int accept(Socket* listenSocket);
        void close();

        int send(const char* data, unsigned int len);
        int recv(char* data, unsigned int len);

        void setSendBuf(unsigned int sendBuf);
        void setRecvBuf(unsigned int recvBuf);

    protected:
        void onEvent(bool read, bool write, bool exception);

    protected:
        SocketListener* listener;
        Epoll* epoll = nullptr;
        int sockFd = -1;
    };
} // namespace One

#endif
