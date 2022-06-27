#include "CppFunc.h"
#include "../../explainer/meta/MetaFunc.h"

CppFunc::CppFunc(MetaFunc* metaFunc, CppContainer* cppContainer)
    :CppBase(cppContainer)
{
    this->metaFunc = metaFunc;
    metaFunc->ptr = this;
}
  
CppFunc* CppFunc::getCppFunc(MetaFunc* metaFunc)
{
    return (CppFunc*)metaFunc->ptr;
}