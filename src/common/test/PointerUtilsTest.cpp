#include "../src/PointerUtils.h"
#include "TestCommon.h"

void test_compare(const void* p1, const void* p2, int verify)
{
    int result = PointerUtils::compare(p1, p2);
    TEST_ASSERT(result == verify, "p1=[%p] p1=[%p] result=[%d] verify=[%d]", p1, p2, result, verify);
}

void test_compare_all()
{
    char c[2];
    test_compare(&c[0], &c[1], -1);
    test_compare(&c[1], &c[0], 1);
    test_compare(&c[1], &c[1], 0);
}

int main()
{
    test_compare_all();

    printf("all successed\n");
    return 0;
}