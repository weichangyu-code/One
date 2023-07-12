#include "SyntaxBase.h"
#include "../explain/ExplainContext.h"

SyntaxBase::SyntaxBase(ExplainContext* context)
{
    //这里面不能保存context，这是在解释的时候临时创建的角色
    if (context)
    {
        context->addSyntaxObject(this);
    }
}

SyntaxBase::~SyntaxBase()
{
    //
}
