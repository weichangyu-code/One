#pragma once

#include "Network.h"

namespace OneCoroutine
{
    SOCKET createSocket(bool tcpOrUdp);
    void setSocketAsync(SOCKET sock);
    void setSocketOption(SOCKET sock, int level, int type, void* value, unsigned int valueLen);
    void getSocketOption(SOCKET sock, int level, int type, void* value, unsigned int valueLen);
    void setSocketLinger(SOCKET sock, int value);
    void setSocketNodelay(SOCKET sock, bool value);     
    void setSocketSendBuf(SOCKET sock, unsigned int size);
    void setSocketRecvBuf(SOCKET sock, unsigned int size);
    unsigned int getSocketSendBuf(SOCKET sock);
    unsigned int getSocketRecvBuf(SOCKET sock);
    void setSocketReuseaddr(SOCKET sock, bool value);
    void setSocketSendTimeout(SOCKET sock, unsigned int timeout);
    void setSocketRecvTimeout(SOCKET sock, unsigned int timeout);
}
