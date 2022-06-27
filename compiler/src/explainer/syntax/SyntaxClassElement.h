#pragma once
#include "SyntaxBase.h"

class SyntaxFunc;
class SyntaxClass;
class SyntaxVarDef;
class SyntaxClassElement : public SyntaxBase
{
public:
    SyntaxClassElement(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxVarDef* varDef = nullptr;
    SyntaxFunc* func = nullptr;
    SyntaxClass* clazz = nullptr;
};
