#include "MetaBase.h"
#include "MetaContainer.h"
#include "../syntax/SyntaxBase.h"

MetaBase::MetaBase(MetaContainer* metaContainer, SyntaxBase* syntaxObj)
{
    this->syntaxObj = syntaxObj;
    if (syntaxObj)
    {
        syntaxObj->ptr = this;
    }

    this->metaContainer = metaContainer;
    metaContainer->addMetaObject(this);
}
    
MetaBase::~MetaBase()
{
    
}
