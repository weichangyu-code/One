#include "MetaBlock.h"
#include "MetaVariable.h"
#include "MetaData.h"
#include "MetaInstruct.h"

MetaBlock::MetaBlock(MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBoxBase(BOX_BLOCK, outer, metaContainer, syntaxObj)
{

}

MetaVariable* MetaBlock::getVariable(const string& name, bool onlyStatic)
{
    for (auto& var : vars)
    {
        if (var->name == name)
        {
            return var;
        }
    }
    return nullptr;
}  

MetaVariable* MetaBlock::addVeriable(const string& name, SyntaxBase* syntaxObj)
{
    MetaVariable* var = new MetaVariable(name, VAR_LOCAL, this, metaContainer, syntaxObj);
    vars.push_back(var);
    return var;
}
