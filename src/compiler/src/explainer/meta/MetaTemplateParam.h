#pragma once
#include "MetaBase.h"
#include "MetaType.h"

class MetaTemplateParam : public MetaBase
{
public:
    MetaTemplateParam(MetaContainer* metaContainer)
        :MetaBase(metaContainer, nullptr)
    {
    }
    MetaTemplateParam(const string& name, MetaContainer* metaContainer)
        :MetaBase(metaContainer, nullptr)
    {
        this->name = name;
    }

    bool haveType()
    {
        return type.isNone() == false;
    }

public:
    string name;
    MetaType type;
};