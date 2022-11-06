#include "../src/SystemUtils.h"
#include "TestCommon.h"

int main()
{
    unsigned long long start = SystemUtils::getUSTick();
    SystemUtils::sleep(900);
    unsigned long long end = SystemUtils::getUSTick();
    unsigned long long len = end - start;
    printf("%d\n", (int)len);
    return 0;
}