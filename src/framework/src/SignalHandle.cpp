#include "SignalHandle.h"
#include "OneException.h"


#ifdef _WIN32

extern "C"
{
#include <eh.h>
}

namespace One
{
    extern "C" void se_translator_function(unsigned int u, struct _EXCEPTION_POINTERS* ep)
    {
        throwNullPointExeption();
    }

    void initSignalHandle()
    {
        //_set_se_translator(se_translator_function);
    }
}

#else

#endif



