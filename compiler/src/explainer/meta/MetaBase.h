#pragma once
#include "Common.h"

class SyntaxBase;
class MetaContainer;
class MetaBase
{
public:
    MetaBase(MetaContainer* metaContainer, SyntaxBase* syntaxObj);
    virtual ~MetaBase();

public:
    MetaContainer* metaContainer = nullptr;
    SyntaxBase* syntaxObj = nullptr;
    void* ptr = nullptr;
};
