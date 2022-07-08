#pragma once
#include "MetaBase.h"
#include "MetaData.h"
#include "../common/Instruct.h"

class MetaBlock;
class MetaFunc;
class MetaVariable;
class MetaInstruct;
class MetaInstruct : public MetaBase
{
public:
    MetaInstruct(MetaContainer* metaContainer, SyntaxBase* syntaxObj)
        :MetaBase(metaContainer, syntaxObj)
    {
        
    }

public:
    int cmd = NOP;
    list<MetaData> params;
    MetaType retType;                           //语句返回的类型

    union
    {
        MetaBlock* block = nullptr;             //
        MetaFunc* func;                         //函数调用和NEW的时候
        MetaVariable* var;
        MetaInstruct* jump;
        void* jumpTmp;
    };

    //转换后的C++代码
    string cppCode;
};


