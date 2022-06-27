#pragma once
#include "MetaBoxBase.h"

class MetaInstruct;
class MetaBlock : public MetaBoxBase
{
public:
    MetaBlock(MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj);

    MetaVariable* getVariable(const string& name, bool onlyStatic);
    MetaVariable* addVeriable(const string& name, SyntaxBase* syntaxObj);

public:
    list<MetaInstruct*> instructs;
    list<MetaVariable*> vars;       //局部变量，临时变量
};


