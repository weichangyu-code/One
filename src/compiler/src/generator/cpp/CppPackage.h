#pragma once
#include "Common.h"
#include "CppBase.h"

class MetaPackage;
class CppPackage : public CppBase
{
public:
    CppPackage(MetaPackage* metaPackage, CppContainer* cppContainer);

    static CppPackage* getCppPackage(MetaPackage* metaPackage);

public:
    MetaPackage* metaPackage = nullptr;

    string cppFolder;
};
