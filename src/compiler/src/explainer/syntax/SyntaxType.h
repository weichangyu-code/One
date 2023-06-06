#pragma once
#include "SyntaxBase.h"

class SyntaxType;
class SyntaxClass;
class SyntaxTypePathItem : public SyntaxBase
{
public:
    SyntaxTypePathItem(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

    SyntaxTypePathItem(const string& typeName, ExplainContext* context)
        :SyntaxBase(context)
    {
        this->typeName = typeName;
    }

public:
    string typeName;
    list<SyntaxType*> templateTypes;
};


class SyntaxType : public SyntaxBase
{
public:
    SyntaxType(ExplainContext* context)
        :SyntaxBase(context)
    {
        
    }

    static SyntaxType* createBoolType(ExplainContext* context)
    {
        SyntaxTypePathItem* item = new SyntaxTypePathItem(context);
        item->typeName = "bool";

        SyntaxType* type = new SyntaxType(context);
        type->items.push_back(item);
        return type;
    }

public:
    list<SyntaxTypePathItem*> items;
    SyntaxClass* anony = nullptr;
    int arrNum = 0;
    bool ellipsis = false;              //...省略号，代表可变参数
};
