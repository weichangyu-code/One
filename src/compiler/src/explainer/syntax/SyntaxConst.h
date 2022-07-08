#pragma once
#include "SyntaxBase.h"

class SyntaxConst : public SyntaxBase
{
public:
    SyntaxConst(ExplainContext* context, int type, const string& value)
        :SyntaxBase(context)
    {
        this->type = type;
        this->value = value;
    }

public:
    enum
    {
        NONE,
        BOOLEAN,
        NUMBER,
        STRING,
        CHAR,
        REFNULL,
    };
    int type = NONE;
    string value;
};
