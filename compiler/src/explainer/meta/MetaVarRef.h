#pragma once
#include "MetaBase.h"
#include "MetaData.h"

class MetaVariable;
class MetaVarRef : public MetaBase
{
public:
    MetaVarRef(MetaContainer* metaContainer);
    MetaVarRef(MetaData obj, MetaContainer* metaContainer);

    MetaType getType();

    void setObj(MetaVariable* obj);
    void addMember(MetaVariable* member);

    static MetaVarRef* makeVarRef(MetaContainer* metaContainer, MetaVarRef* ref, MetaVariable* var);

public:
    MetaData obj;
    MetaType type;                      //最终类型
    list<MetaData> indexes;             //表示连环索引，例如var[0].ptr
};

