#pragma once

#ifdef _WIN32

#include "../Network.h"
#include <functional>
#include "List.h"
using namespace OneCommon;

namespace OneCoroutine
{
    class OperateOverlapped;
    typedef std::function<void(OperateOverlapped* oo)> OOCompleteCB;

    class OperateOverlapped
    {
    public:
        OVERLAPPED ol;

        int trans = 0;
        int error = 0;
        char* buffer = nullptr;

        HANDLE handle = INVALID_HANDLE_VALUE;
        OOCompleteCB cb;
        OOCompleteCB cbUser;

        ListNode poolNode;
    };
}

#endif
