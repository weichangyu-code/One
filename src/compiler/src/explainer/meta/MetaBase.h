#pragma once
#include "Common.h"

class SyntaxBase;
class MetaContainer;
class MetaBase
{
public:
    MetaBase(MetaContainer* metaContainer, SyntaxBase* syntaxObj);
    virtual ~MetaBase();

    void bind(SyntaxBase* syntaxObj);

public:
    MetaContainer* metaContainer = nullptr;
    SyntaxBase* syntaxObj = nullptr;
    void* ptr = nullptr;
};
