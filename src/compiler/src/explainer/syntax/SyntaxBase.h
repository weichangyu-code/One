#pragma once
#include "Common.h"

class ExplainContext;
class SyntaxBase
{
public:
    SyntaxBase(ExplainContext* context);
    virtual ~SyntaxBase();

public:
    void* ptr = nullptr;                    //解析的时候用的
};
