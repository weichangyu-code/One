#include "MetaFunc.h"
#include "MetaVariable.h"
#include "MetaClass.h"
#include "../syntax/SyntaxFunc.h"
#include "../syntax/SyntaxClass.h"
#include "../common/Keyword.h"
#include "StringUtils.h"
using namespace OneCommon;

MetaFunc::MetaFunc(const string& name, MetaClass* clazz, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBoxBase(BOX_FUNC, clazz, metaContainer, syntaxObj)
{
    this->name = name;
    this->return_.setType(DT_VOID);

    SyntaxFunc* syntaxFunc = (SyntaxFunc*)syntaxObj;
    if (syntaxFunc)
    {
        this->funcType = syntaxFunc->type;
        this->isStatic = syntaxFunc->isStatic;
        this->isVirtual = syntaxFunc->isVirtual;
    }
}

MetaVariable* MetaFunc::addParam(const string& name, SyntaxBase* syntaxObj)
{
    MetaVariable* var = new MetaVariable(name, VAR_PARAM, this, metaContainer, syntaxObj);
    params.push_back(var);
    return var;
}

MetaVariable* MetaFunc::addParam(const string& name, MetaType& type)
{
    MetaVariable* var = new MetaVariable(name, VAR_PARAM, this, metaContainer, nullptr);
    var->type = type;
    params.push_back(var);
    return var;
}
    
MetaClass* MetaFunc::addAnonyClass(MetaBoxBase* box, SyntaxBase* syntaxObj)
{
    string name = "Anony" + StringUtils::itoa(anonyClasses.size());
    SyntaxClass* syntaxClass = (SyntaxClass*)syntaxObj;
    syntaxClass->name = name;

    MetaClass* clazz = new MetaClass(name, box, metaContainer, syntaxObj);
    clazz->isAnonyClass = true;
    anonyClasses.push_back(clazz);
    return clazz;
}

MetaVariable* MetaFunc::getVariable(const string& name, bool onlyStatic)
{
    for (auto& param : params)
    {
        if (param->name == name)
        {
            return param;
        }
    }
    return nullptr;
}
    
bool MetaFunc::isInitVarFunc()
{
    return this->funcType == FUNC_INIT || this->funcType == FUNC_STATIC_INIT;
}
