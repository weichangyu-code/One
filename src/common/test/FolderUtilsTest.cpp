#include "../src/FolderUtils.h"
#include "TestCommon.h"

int main()
{
    FolderUtils::findEach("./", [](const string& name, bool isDir) {
        printf("%s\n", name.c_str());
        return true;
    });

    FolderUtils::removeDir("./tmp");

    return 0;
}