#include "MetaVariable.h"
#include "../syntax/SyntaxVarDef.h"
#include "../common/Keyword.h"

MetaVariable::MetaVariable(const string& name, int varType, MetaBoxBase* box, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBase(metaContainer, syntaxObj)
{
    this->name = name;
    this->varType = varType;
    this->box = box;

    if (syntaxObj)
    {
        SyntaxVarDef* syntaxVarDef = (SyntaxVarDef*)syntaxObj;
        this->isStatic = syntaxVarDef->isStatic;
        this->isConst = syntaxVarDef->isConst;
        this->isDynamic = syntaxVarDef->isDynamic;
    }
}