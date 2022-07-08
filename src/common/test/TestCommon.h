#pragma once

#define TEST_ASSERT(b, ...) \
printf(__FUNCTION__); \
if (!(b)) \
{ \
    printf(" failed:" __VA_ARGS__); \
    exit(-1); \
} \
else \
{ \
    printf(" successed:" __VA_ARGS__); \
} \
printf("\n");
