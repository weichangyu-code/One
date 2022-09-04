#pragma once
#include "SyntaxBase.h"
#include "SyntaxMulti.h"

class SyntaxType;
class SyntaxExp;
class SyntaxVarDef : public SyntaxBase
{
public:
    SyntaxVarDef(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

    void addVarDefAndAssginInstruct(ExplainContext* context);
    void addAssginInstruct(ExplainContext* context);

public:
    bool isConst = false;
    bool isStatic = false;

    //类型，null代表auto
    SyntaxType* type = nullptr;

    //名字，为空表示临时变量
    string name;

    //包含赋值语句和定义语句
    //只包含赋值表达式
    SyntaxExp* exp = nullptr;
};
