#pragma once

namespace OneCommon
{
    class WatchTimer
    {
    public:
        WatchTimer(unsigned int interval);

        void setInterval(unsigned int interval);
        void active();
        bool check();

    public:
        unsigned int interval = 0;
        unsigned int lastTick = 0;
    };
}

