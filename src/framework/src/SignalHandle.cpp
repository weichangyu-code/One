#include "SignalHandle.h"
#include "OneException.h"
#include "ObjectPool.h"


#ifdef _MSC_VER
#include <eh.h>

namespace One
{
    extern "C" void se_translator_function(unsigned int u, struct _EXCEPTION_POINTERS* ep)
    {
        throw g_objectPool.createObjectR<NullPointerException>();
    }

    void initSignalHandle()
    {
        _set_se_translator(se_translator_function);
    }
}

#else

#include <signal.h>

namespace One
{
    void signalHanle(int sig)
    {
        throw g_objectPool.createObjectR<NullPointerException>();
    }

    void initSignalHandle()
    {
        signal(SIGSEGV, signalHanle);
    }
}


#endif


