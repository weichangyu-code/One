#pragma once
#include "Common.h"
#include "../../common/Result.h"
#include "OByteStream.h"

class MetaPackage;
class MetaClass;
class MetaContainer;
class Dumper
{
public:
    Result dump(MetaContainer* container, const string& path);

protected:
    Result dumpPackage(MetaPackage* package);
    Result dumpClass(MetaClass* clazz);

    Result toCppFile(const string& path);

protected:
    OByteStream _stream;
};
