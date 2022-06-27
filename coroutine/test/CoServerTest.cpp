#include "../src/engine/Engine.h"
#include "../src/engine/CoCondition.h"
#include "../src/engine/CoMutex.h"
#include "../src/engine/CoSocket.h"
#include "../src/engine/CoSystem.h"
#include "../src/engine/CoStdOut.h"
#include "SystemUtils.h"
#include "WatchTimer.h"
#include <stdio.h>
using namespace OneCommon;
using namespace OneCoroutine;

int main()
{
    WatchTimer timer(1000);
    unsigned int recvSumLen = 0;
    const static unsigned int WATCH_NUM = 10;
    unsigned int recvLen[WATCH_NUM] = {0};

    Engine engine;
    engine.createCoroutine([&timer, &recvSumLen, &recvLen](Coroutine* co){
        CoSocket listen;
        if (listen.listen("0.0.0.0", 12346, true, 128) != 0)
        {
            CoStdOut::print("listen failed\n");
            return;
        }
        CoStdOut::print("listen success\n");

        CoSocket* newSocket = new CoSocket();
        int i = 0;
        while (1)
        {
            if (newSocket->accept(&listen) == 0)
            {
                CoSystem::createCoroutine([newSocket, i, &timer, &recvSumLen, &recvLen](Coroutine* co) {
                    CoStdOut::print("accept %d\n", i);
                    while (1)
                    {
                        static char buf[4096];
                        int len = newSocket->recv(buf, sizeof(buf));
                        if (len < 0)
                        {
                            CoStdOut::print("reset %d\n", i);
                            break;
                        }
                        recvSumLen += len;
                        if (i % 10 == 0)
                        {
                            int index = i / 10;
                            if (index < WATCH_NUM)
                            {
                                recvLen[index] += len;
                            }
                        }
                        if (timer.check())
                        {
                            CoStdOut::print("recv sum %d\n", recvSumLen);
                            for (int ii = 0;ii < WATCH_NUM;ii++)
                            {
                                CoStdOut::print("recv %d、", recvLen[ii]);
                                recvLen[ii] = 0;
                            }
                            CoStdOut::print("\n");
                            recvSumLen = 0;
                        }
                    }
                    delete newSocket;
                });

                //new一个，继续监听
                i++;
                newSocket = new CoSocket();
            }
        }
    });
    engine.run();

    return 0;
}