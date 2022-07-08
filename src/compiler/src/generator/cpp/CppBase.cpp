#include "CppBase.h"
#include "CppContainer.h"

CppBase::CppBase(CppContainer* cppContainer)
{
    cppContainer->addCppObject(this);
}
    
CppBase::~CppBase()
{

}