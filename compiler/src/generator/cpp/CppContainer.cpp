#include "CppContainer.h"
#include "CppBase.h"

CppContainer::CppContainer()
{

}

CppContainer::~CppContainer()
{
    for (auto& obj : cppObjs)
    {
        delete obj;
    }
}
    
void CppContainer::addCppObject(CppBase* obj)
{
    cppObjs.push_back(obj);
}