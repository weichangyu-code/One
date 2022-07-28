#include "../src/engine/Engine.h"
#include "../src/engine/CoCondition.h"
#include "../src/engine/CoMutex.h"
#include "../src/engine/CoSocket.h"
#include "../src/engine/CoSystem.h"
#include "../src/engine/CoStdOut.h"
#include "../src/engine/CoEvent.h"
#include "SystemUtils.h"
#include "WatchTimer.h"
#include <stdio.h>
using namespace OneCommon;
using namespace OneCoroutine;

int main()
{
    WatchTimer timer(1000);
    unsigned int sendSumLen = 0;

    Engine engine;
    CoEvent event(false);
    engine.createCoroutine([&timer, &sendSumLen, &event](Coroutine* co){
        for (int i = 0;i < 3;i++)
        {
            if (i % 1000 == 0)
            {
                //1秒1000个连接
                CoSystem::sleep(1000);
            }
            CoSystem::createCoroutine([i, &timer, &sendSumLen, &event](Coroutine* co) {
                CoSocket newSocket;
                //int ret = newSocket.connect("10.10.4.111", 12345, 10000);
                //int ret = newSocket.connect("10.12.1.189", 12346, 30000);
                int ret = newSocket.connect("127.0.0.1", 12346, 10000);
                if (ret < 0)
                {
                    CoStdOut::print("connect failed %d\n", i);
                    return;
                }
                CoStdOut::print("connect success %d\n", i);
                //CoSystem::sleep(100000);
                //event.wait();
                while (1)
                {
                    static char buf[4096];
                    int len = newSocket.send(buf, sizeof(buf));
                    if (len < 0)
                    {
                        CoStdOut::print("connect reset %d\n", i);
                        break;
                    }
                    sendSumLen += len;
                    if (timer.check())
                    {
                        CoStdOut::print("send %d\n", sendSumLen);
                        sendSumLen = 0;
                    }
                }
            });
        }
        CoSystem::sleep(1000);
        //event.signal();
    });
    engine.run();
    printf("end\n");

    return 0;
}