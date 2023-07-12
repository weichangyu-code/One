#include "MetaBlock.h"
#include "MetaVariable.h"
#include "MetaData.h"
#include "MetaInstruct.h"
#include "MetaFunc.h"
#include "../common/Member.h"
#include "MetaContainer.h"
#include "StringUtils.h"
using namespace OneCommon;

MetaBlock::MetaBlock(MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBoxBase(BOX_BLOCK, outer, metaContainer, syntaxObj)
{

}

MetaVariable* MetaBlock::getVariable(const string& name, int filterType)
{
    for (auto& var : vars)
    {
        if (var->name == name && filterMember(var->isStatic, filterType))
        {
            return var;
        }
    }
    return nullptr;
}  

MetaVariable* MetaBlock::addVariable(const string& name, SyntaxBase* syntaxObj)
{
    MetaVariable* var = new MetaVariable(name, VAR_LOCAL, this, metaContainer, syntaxObj);
    vars.push_back(var);
    return var;
}

MetaVariable* MetaBlock::createAnonyVariable()
{
    MetaFunc* outFunc = getOuterFunc();
    string name = "__anonyVar" + StringUtils::itoa((int)outFunc->anaonyVariables.size()) + "__";
    MetaVariable* var = new MetaVariable(name, VAR_ANONY, this, metaContainer, nullptr);
    outFunc->anaonyVariables.push_back(var);
    return var;
}
    
void MetaBlock::addInstruct(MetaInstruct* instruct)
{
    instructs.push_back(instruct);
}
    
void MetaBlock::removeInstruct(MetaInstruct* instruct)
{
    auto iter = std::find(instructs.begin(), instructs.end(), instruct);
    assert(iter != instructs.end());
    instructs.erase(iter);
}
