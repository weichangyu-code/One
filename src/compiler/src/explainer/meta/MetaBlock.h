#pragma once
#include "MetaBoxBase.h"
#include "MetaData.h"

class MetaInstruct;
class MetaBlock : public MetaBoxBase
{
public:
    MetaBlock(MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj);

    MetaVariable* getVariable(const string& name, int filterType);
    MetaVariable* addVariable(const string& name, SyntaxBase* syntaxObj);
    MetaVariable* createAnonyVariable();

    void addInstruct(MetaInstruct* instruct);
    void removeInstruct(MetaInstruct* instruct);

public:
    list<MetaInstruct*> instructs;
    list<MetaVariable*> vars;       //局部变量，临时变量
};


