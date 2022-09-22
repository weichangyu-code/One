#include "MetaVarRef.h"
#include "MetaVariable.h"
#include "MetaClass.h"
#include "MetaTemplateParam.h"

MetaVarRef::MetaVarRef(MetaContainer* metaContainer)
    :MetaBase(metaContainer, nullptr)
{
}
    
MetaVarRef::MetaVarRef(MetaData obj, MetaContainer* metaContainer)
    :MetaBase(metaContainer, nullptr)
{
    this->obj = obj;
    this->type = obj.getType();
}

MetaType MetaVarRef::getType() const
{
    return type;
}
    
void MetaVarRef::addMember(MetaVariable* member)
{
    this->indexes.push_back(MetaData(MetaData::MEMBER, member));
    this->type = member->type;
}
    
void MetaVarRef::addIndex(const MetaData& index)
{
    this->indexes.push_back(index);
    this->type = this->type.clazz->params.front()->type;
}
    
MetaVarRef* MetaVarRef::makeVarRef(MetaContainer* metaContainer, MetaVarRef* ref, MetaVariable* var)
{
    if (var->varType == VAR_LOCAL || var->varType == VAR_PARAM || var->isStatic)
    {
        //局部变量，静态变量，新的引用
        ref = new MetaVarRef(var, metaContainer);
    }
    else if (var->varType == VAR_THIS || var->varType == VAR_SUPER)
    {
        if (ref == nullptr)
        {
            ref = new MetaVarRef(var, metaContainer);
        }
        else
        {
            ref->addMember(var);
        }
    }
    else
    {
        //成员变量
        if (ref == nullptr)
        {
            //ref = new MetaVarRef(var->box->getOuterClass()->getThisVariable(), metaContainer);
            ref = new MetaVarRef(var, metaContainer);
        }
        else
        {
            ref->addMember(var);
        }
    }
    return ref;
}
