#include "CppClass.h"
#include "../../explainer/meta/MetaClass.h"
#include "../../explainer/meta/MetaTemplateParam.h"
#include "CppFile.h"
#include <sstream>

CppClass::CppClass(MetaClass* metaClass, CppContainer* cppContainer)
    :CppBase(cppContainer)
{
    this->metaClass = metaClass;
    metaClass->ptr = this;
}
    
CppClass* CppClass::getCppClass(MetaClass* metaClass)
{
    return (CppClass*)metaClass->ptr;
}

CppClass* CppClass::getCppClass(MetaClass* metaClass, CppContainer* cppContainer)
{
    if (metaClass->ptr == nullptr)
    {
        new CppClass(metaClass, cppContainer);
    }
    return getCppClass(metaClass);
}

string CppClass::getFactoryName()
{
    return cppName + "Factory";
}

string CppClass::getTemplateParamDefine()
{
    if (metaClass->params.empty() == false)
    {
        ostringstream stream; 
        stream << "template<";
        for (auto& param : metaClass->params)
        {
            if (param != metaClass->params.front())
            {
                stream << ", ";
            }
            stream << "typename " << param->name;
        }
        stream << ">";
        return stream.str();
    }
    else
    {
        return "";
    }
}
    
string CppClass::getTemplateParamNames()
{
    if (metaClass->params.empty() == false)
    {
        ostringstream stream; 
        stream << "<";
        for (auto& param : metaClass->params)
        {
            if (param != metaClass->params.front())
            {
                stream << ", ";
            }
            stream << param->name;
        }
        stream << ">";
        return stream.str();
    }
    else
    {
        return "";
    }
}
    
string CppClass::getTemplateParamImpl()
{
    if (metaClass->params.empty() == false)
    {
        ostringstream stream;
        stream << "<";
        for (auto& param : metaClass->params)
        {
            if (param != metaClass->params.front())
            {
                stream << ", ";
            }
            if (param->type.isBaseType())
            {
                stream << MetaType::typeToCppString(param->type.type);
            }
            else
            {
                stream << CppClass::getCppClass(param->type.clazz)->cppName;
            }
        }
        stream << ">";
        return stream.str();
    }
    else
    {
        return "";
    }
}
