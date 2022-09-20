#pragma once
#include "MetaBoxBase.h"
#include "MetaType.h"
#include "../common/VarType.h"

class MetaVariable : public MetaBase
{
public:
    MetaVariable(const string& name, int varType, MetaBoxBase* box, MetaContainer* metaContainer, SyntaxBase* syntaxObj);

public:
    string name;
    int varType = VAR_LOCAL;

    bool isStatic = false;
    bool isConst = false;
    bool isDynamic = false;

    MetaType type;                          //NONE表示自动
    MetaBoxBase* box = nullptr;

    MetaBlock* initBlock = nullptr;         //const变量要用
};

