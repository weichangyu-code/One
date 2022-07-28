#pragma once

namespace OneCoroutine
{
    enum
    {
        ERR_SUCCESS = 0,
        ERR_ERROR = -1,

        ERR_SOCKET_TIMEOUT = -10,
        ERR_SOCKET_AGAIN = -11,                  //忙，重试
        ERR_SOCKET_INPROGRESS = -12,             //表示正在连接
        ERR_SOCKET_BIND_FAILED = -13,

        ERR_FILE_NOT_FOUND = -20,
        ERR_FILE_ACCESS_DENIED = -21,
    };
}

