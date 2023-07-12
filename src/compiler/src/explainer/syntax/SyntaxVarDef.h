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

public:
    bool isConst = false;
    bool isStatic = false;
    bool isDynamic = false;        //可变参数，代表多个参数

    //类型，null代表auto
    SyntaxType* type = nullptr;

    //名字，为空表示临时变量
    string name;

    //包含赋值语句和定义语句
    //只包含赋值表达式
    bool haveInitExp = false;
    SyntaxExp* initExp = nullptr;      //如果已经消化会职位
};
