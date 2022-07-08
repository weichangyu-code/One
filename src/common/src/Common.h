#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <memory.h>
#include <stdio.h>
#include <string.h>
using namespace std;

#define SAFE_DELETE(ptr) \
if (ptr) \
{ \
    delete (ptr); \
    (ptr) = nullptr; \
}

#define UINT_COMPARE(a, b)		    (((int)(a) - (int)(b)))
#define UINT64_COMPARE(a, b)		(((long long)(a) - (long long)(b)))