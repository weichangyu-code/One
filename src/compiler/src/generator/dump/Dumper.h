#pragma once
#include "Common.h"
#include "../../common/Result.h"
#include "OByteStream.h"

class MetaPackage;
class MetaClass;
class MetaContainer;
class MetaFunc;
class MetaVariable;
class Dumper
{
public:
    Result dump(MetaContainer* container, const string& path);

protected:
    Result dumpPackage(MetaPackage* package);
    Result dumpClass(MetaClass* clazz);
    Result dumpFunc(MetaFunc* func);
    Result dumpField(MetaVariable* var);

    Result toCppFile(const string& path);

protected:
    OByteStream _stream;
};
