#include "CoStdOut.h"
#include <stdio.h>
#include <stdarg.h>
#include "Engine.h"

namespace OneCoroutine
{
    void CoStdOut::print(const char* fmt, ...)
    {
		va_list ap;
		va_start(ap, fmt);
        
        //TODO:先放线程池里，后续用异步IO替代
        Engine::getCurEngine()->executeOnPool([&ap, fmt]() {
            vprintf(fmt, ap);
        });

		va_end(ap);
    }
} // namespace One
