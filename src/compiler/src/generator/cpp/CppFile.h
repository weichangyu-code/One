#pragma once
#include "Common.h"
#include "CppBase.h"

class MetaFile;
class CppFile : public CppBase
{
public:
    CppFile(MetaFile* metaFile, CppContainer* cppContainer);

    static CppFile* getCppFile(MetaFile* metaFile);

public:
    MetaFile* metaFile = nullptr;
};