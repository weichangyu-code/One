#include "MetaBase.h"
#include "MetaContainer.h"
#include "../syntax/SyntaxBase.h"

MetaBase::MetaBase(MetaContainer* metaContainer, SyntaxBase* syntaxObj)
{
    bind(syntaxObj);

    this->metaContainer = metaContainer;
    metaContainer->addMetaObject(this);
}
  
MetaBase::~MetaBase()
{
    
}
    
void MetaBase::bind(SyntaxBase* syntaxObj)
{
    this->syntaxObj = syntaxObj;
    if (syntaxObj)
    {
        syntaxObj->ptr = this;
    }
}
