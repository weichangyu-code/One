#include "../src/CollectionUtils.h"
#include "TestCommon.h"

int main()
{
    list<string> strs1;
    strs1.push_back("aaaaa");
    list<string> strs2;
    strs2.push_back("bbbbb");
    CollectionUtils::compare(strs1, strs2, [](list<string>::const_reference v1, list<string>::const_reference v2){
        return v1.compare(v2);
    });

    return 0;
}