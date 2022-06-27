#include "MetaVarRef.h"
#include "MetaVariable.h"
#include "MetaClass.h"

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

MetaType MetaVarRef::getType()
{
    return type;
}
    
void MetaVarRef::setObj(MetaVariable* obj)
{
    this->obj.setData(obj);
}
    
void MetaVarRef::addMember(MetaVariable* member)
{
    this->indexes.push_back(MetaData(MetaData::MEMBER, member));
    this->type = member->type;
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
            ref = new MetaVarRef(var->box->getOuterClass()->getThisVariable(), metaContainer);
        }
        ref->addMember(var);
    }
    return ref;
}
