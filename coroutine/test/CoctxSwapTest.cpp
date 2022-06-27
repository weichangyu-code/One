
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include "../src/coctx/coctx.h"
#include "SystemUtils.h"
using namespace OneCommon;

const int stackSize = 1024*128;
char* stackBuf = new char[stackSize];
coctx_t coctx;
coctx_t coctxMain;
int switchCount = 0;

void my_coroutine_fun(void* s1, void* s2)
{
    while (1) 
    {
        one_coctx_swap(&coctx, &coctxMain);
        switchCount++;
    }
}

int main()
{
    one_coctx_init(&coctxMain);
    one_coctx_make(&coctx, stackBuf, stackSize, my_coroutine_fun, "1", "2");

    unsigned int start = SystemUtils::getMSTick();

    for (int i = 0;i < 10000000;i++)
    {
        one_coctx_swap(&coctxMain, &coctx);
        switchCount++;
    }

    printf("use time %d i=%d\n", SystemUtils::getMSTick() - start, switchCount);

    return 0;
}