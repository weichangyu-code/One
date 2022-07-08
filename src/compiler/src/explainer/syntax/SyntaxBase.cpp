#include "SyntaxBase.h"
#include "../explain/ExplainContext.h"

SyntaxBase::SyntaxBase(ExplainContext* context)
{
    if (context)
    {
        context->addSyntaxObject(this);
    }
}

SyntaxBase::~SyntaxBase()
{
    //
}
