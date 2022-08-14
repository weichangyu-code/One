
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include "Common.h"
#include "SystemUtils.h"
#include "thread/Event.h"
#include "thread/ThreadPool.h"
#include <unistd.h>
#include <semaphore.h>
using namespace std;
using namespace OneCommon;
using namespace OneCoroutine;


// int main()
// {
//     mutex mtx1;
//     condition_variable cond1;
//     mutex mtx2;
//     condition_variable cond2;
//     mutex mtx3;
//     condition_variable cond3;
//     atomic_int times1;
//     atomic_int times2;

//     new thread([&]() {
//         unique_lock<mutex> l(mtx1);
//         while (times1.fetch_add(1) < 10000)
//         {
//             cond1.wait(l);

//             mtx2.lock();
//             mtx2.unlock();
//             cond2.notify_one();
//         }
        
//         mtx2.lock();
//         cond2.notify_one();
//         mtx2.unlock();

//         cond3.notify_one();
//     });

//     new thread([&]() {
//         unique_lock<mutex> l(mtx2);
//         while (times2.fetch_add(1) < 10000)
//         {
//             cond2.wait(l);

//             mtx1.lock();
//             mtx1.unlock();
//             cond1.notify_one();
//         }
        
//         mtx1.lock();
//         cond1.notify_one();
//         mtx1.unlock();

//         cond3.notify_one();
//     });

//     SystemUtils::sleep(1000);

//     unsigned int start = SystemUtils::getMSTick();
//     cond1.notify_one();
//     unique_lock<mutex> l(mtx3);
//     cond3.wait(l);
//     l.unlock();
//     printf("use time %d. times1=%d times2=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load());

//     return 0;
// }

// int main()
// {
//     Event event1;
//     Event event2;
//     Event event3;

//     atomic_int times1;
//     atomic_int times2;

//     new thread([&]() {
//         while (times1.fetch_add(1) < 100000)
//         {
//             event1.wait();

//             event2.signal();
//         }
//         event3.signal();
//     });

//     new thread([&]() {
//         while (times2.fetch_add(1) < 100000)
//         {
//             event2.wait();

//             event1.signal();
//         }
//         event3.signal();
//     });

//     SystemUtils::sleep(1000);

//     unsigned int start = SystemUtils::getMSTick();
//     event1.signal();
//     event3.wait();
//     printf("use time %d. times1=%d times2=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load());

//     return 0;
// }


// int main()
// {
//     int fd1[2];
//     int fd2[2];

//     pipe(fd1);
//     pipe(fd2);

//     Event event3;
//     atomic_int times1;
//     atomic_int times2;

//     new thread([&]() {
//         while (times1.fetch_add(1) < 100000)
//         {
//             char buf = 0;
//             int ret = read(fd1[0], &buf, 1);

//             write(fd2[1], "", 1);
//         }
//         event3.signal();
//     });

//     new thread([&]() {
//         while (times2.fetch_add(1) < 100000)
//         {
//             char buf = 0;
//             read(fd2[0], &buf, 1);

//             write(fd1[1], "", 1);
//         }
//         event3.signal();
//     });

//     SystemUtils::sleep(1000);

//     unsigned int start = SystemUtils::getMSTick();
//     write(fd1[1], "", 1);
//     event3.wait();
//     printf("use time %d. times1=%d times2=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load());

//     return 0;
// }

//用taskset -c 0 ./ThreadSwitchTest命令启动
// int main()
// {
//     Event event1;
//     Event event2;
//     Event event3;

//     atomic_int times1(0);
//     atomic_int times2(0);
//     atomic_int times3(0);

//     new thread([&]() {
//         event1.wait();
//         while (times1.fetch_add(1) < 100000)
//         {
//             SystemUtils::yield();
//             while (times2.load() != times1.load())
//             {
//                 SystemUtils::yield();
//                 times3.fetch_add(1);
//             }
//         }
//         event3.signal();
//     });

//     new thread([&]() {
//         event2.wait();
//         while (times2.fetch_add(1) < 100000)
//         {
//             SystemUtils::yield();
//             while (times2.load() + 1 != times1.load())
//             {
//                 SystemUtils::yield();
//                 times3.fetch_add(1);
//             }
//         }
//         event3.signal();
//     });

//     SystemUtils::sleep(1000);

//     printf("times1=%d times2=%d times3=%d\n", (int)times1.load(), (int)times2.load(), (int)times3.load());

//     unsigned int start = SystemUtils::getMSTick();
//     event1.signal();
//     event2.signal();
//     event3.wait();
//     printf("use time %d. times1=%d times2=%d times3=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load(), times3.load());

//     return 0;
// }


// int main()
// {
//     Event event1;
//     Event event2;
//     Event event3;

//     mutex mtx;

//     atomic_int times1(0);
//     atomic_int times2(0);
//     atomic_int times3(0);

//     new thread([&]() {
//         event1.wait();
//         mtx.lock();
//         while (times1.fetch_add(1) < 100000)
//         {
//             mtx.unlock();
//             while (times2.load() != times1.load())
//             {
//                 SystemUtils::yield();
//                 times3.fetch_add(1);
//             }
//             mtx.lock();
//         }
//         event3.signal();
//     });

//     new thread([&]() {
//         event2.wait();
//         mtx.lock();
//         while (times2.fetch_add(1) < 100000)
//         {
//             mtx.unlock();
//             while (times2.load() + 1 != times1.load())
//             {
//                 SystemUtils::yield();
//                 times3.fetch_add(1);
//             }
//             mtx.lock();
//         }
//         event3.signal();
//     });

//     SystemUtils::sleep(1000);

//     printf("times1=%d times2=%d times3=%d\n", (int)times1.load(), (int)times2.load(), (int)times3.load());

//     unsigned int start = SystemUtils::getMSTick();
//     event1.signal();
//     event2.signal();
//     event3.wait();
//     printf("use time %d. times1=%d times2=%d times3=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load(), times3.load());

//     return 0;
// }

// int main()
// {
//     sem_t sem1;
//     sem_t sem2;

//     sem_init(&sem1, 0, 0);
//     sem_init(&sem2, 0, 0);

//     Event event3;
//     atomic_int times1;
//     atomic_int times2;

//     new thread([&]() {
//         while (times1.fetch_add(1) < 100000)
//         {
//             sem_wait(&sem1);

//             sem_post(&sem2);
//         }
//         event3.signal();
//     });

//     new thread([&]() {
//         while (times2.fetch_add(1) < 100000)
//         {
//             sem_wait(&sem2);

//             sem_post(&sem1);
//         }
//         event3.signal();
//     });

//     SystemUtils::sleep(1000);

//     unsigned int start = SystemUtils::getMSTick();
//     sem_post(&sem1);
//     event3.wait();
//     printf("use time %d. times1=%d times2=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load());

//     return 0;
// }


// int main()
// {
//     sem_t sem1;
//     sem_t sem2;

//     sem_init(&sem1, 0, 0);
//     sem_init(&sem2, 0, 0);

//     Event event3;
//     atomic_int times1;
//     atomic_int times2;

//     ThreadPool tp;

//     new thread([&]() {
//         while (times1.fetch_add(1) < 100000)
//         {
//             sem_wait(&sem1);

//             tp.execute([&sem1]() {
//                 sem_post(&sem1);
//             });
//         }
//         event3.signal();
//     });

//     SystemUtils::sleep(1000);

//     unsigned int start = SystemUtils::getMSTick();
//     sem_post(&sem1);
//     event3.wait();
//     printf("use time %d. times1=%d times2=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load());

//     return 0;
// }


int main()
{
    sem_t sem1;
    sem_t sem2;

    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    Event event3;
    atomic_int times1;
    atomic_int times2;

    ThreadPool tp;

    new thread([&]() {
        while (times1.fetch_add(1) < 100000)
        {
            sem_wait(&sem1);

            tp.execute([&sem1]() {
                sem_post(&sem1);
            });
        }
        event3.signal();
    });

    SystemUtils::sleep(1000);

    unsigned int start = SystemUtils::getMSTick();
    sem_post(&sem1);
    event3.wait();
    printf("use time %d. times1=%d times2=%d\n", SystemUtils::getMSTick() - start, times1.load(), times2.load());

    return 0;
}
