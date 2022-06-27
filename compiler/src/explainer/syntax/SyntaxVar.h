
#pragma once
#include "SyntaxBase.h"

class SyntaxExp;
class SyntaxTypePathItem;
class SyntaxVar : public SyntaxBase
{
public:
    SyntaxVar(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    SyntaxExp* exp = nullptr;
    list<SyntaxTypePathItem*> items;        //变量名，变量名的前面可能是类型或者表达式
};
