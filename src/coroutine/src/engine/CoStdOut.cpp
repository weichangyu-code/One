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
        
        Engine::getCurEngine()->executeOnMain([&ap, fmt]() {
            vprintf(fmt, ap);
        });

		va_end(ap);
    }
} // namespace One
