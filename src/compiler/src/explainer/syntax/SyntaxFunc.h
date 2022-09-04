#pragma once
#include "SyntaxBase.h"
#include "SyntaxMulti.h"
#include "../common/FuncType.h"

class SyntaxType;
class SyntaxVarDef;
class SyntaxBlock;
class SyntaxFunc : public SyntaxBase
{
public:
    SyntaxFunc(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

public:
    int type = FUNC_NORMAL;

    bool isVirtual = false;
    bool isStatic = false;
    bool isAnony = false;

    string name;                            //构造函数就是类名，析构函数是~类名
    SyntaxType* returnType = nullptr;
    list<SyntaxVarDef*> params;
    SyntaxBlock* block = nullptr;           //block为null表示虚函数
};