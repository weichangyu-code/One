#include "CppPackage.h"
#include "../../explainer/meta/MetaPackage.h"

CppPackage::CppPackage(MetaPackage* metaPackage, CppContainer* cppContainer)
    :CppBase(cppContainer)
{
    this->metaPackage = metaPackage;
    metaPackage->ptr = this;
}

CppPackage* CppPackage::getCppPackage(MetaPackage* metaPackage)
{
    return (CppPackage*)metaPackage->ptr;
}