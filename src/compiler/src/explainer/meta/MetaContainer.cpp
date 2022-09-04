#include "MetaContainer.h"
#include "MetaPackage.h"
#include "StringUtils.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "MetaFunc.h"
#include "MetaFile.h"
#include "MetaVariable.h"
#include "MetaVarRef.h"
#include "../common/Keyword.h"
using namespace OneCommon;

MetaContainer::MetaContainer()
{
    rootPackage = new MetaPackage("", nullptr, this);
}

MetaContainer::~MetaContainer()
{
    for (auto& obj : metaObjs)
    {
        delete obj;
    }
}

void MetaContainer::addMetaObject(MetaBase* metaObj)
{
    metaObjs.push_back(metaObj);
}

MetaPackage* MetaContainer::getPackage(const string& path, bool add)
{
    list<string> items = StringUtils::splitString(path, ".");
    return getPackage(items, add);
}

MetaPackage* MetaContainer::getPackage(const list<string>& path, bool add)
{
    MetaPackage* package = rootPackage;
    for (auto& item : path)
    {
        MetaPackage* subPackage = package->getPackage(item, add);
        if (subPackage == nullptr)
        {
            return nullptr;
        }
        package = subPackage;
    }
    return package;
}
    
MetaClass* MetaContainer::getClass(const string& path)
{
    list<string> items = StringUtils::splitString(path, ".");
    if (items.empty())
    {
        return nullptr;
    }

    string className = items.back();
    items.pop_back();
    MetaPackage* package = getPackage(items, false);
    if (package == nullptr)
    {
        return nullptr;
    }
    
    return package->getClass(className);
}
    
MetaClass* MetaContainer::getStringClass()
{
    if (stringClass == nullptr)
    {
        stringClass = getClass(KEY_ONE_STRING_CLASS);
    }
    return stringClass;
}

MetaClass* MetaContainer::getObjectClass()
{
    if (objectClass == nullptr)
    {
        objectClass = getClass(KEY_ONE_OBJECT_CLASS);
    }
    return objectClass;
}
    
MetaClass* MetaContainer::getInterfaceClass()
{
    if (interfaceClass == nullptr)
    {
        interfaceClass = getClass(KEY_ONE_INTERFACE_CLASS);
    }
    return interfaceClass;
}

MetaClass* MetaContainer::getArrayClass()
{
    if (arrayClass == nullptr)
    {
        arrayClass = getClass(KEY_ONE_ARRAY_CLASS);
    }
    return arrayClass;
}

MetaClass* MetaContainer::getIterableClass()
{
    if (iterableClass == nullptr)
    {
        iterableClass = getClass(KEY_ONE_ITERABLE_CLASS);
    }
    return iterableClass;
}

bool MetaContainer::isArray(const MetaType& type)
{
    if (type.type != DT_CLASS || type.clazz == nullptr)
    {
        return false;
    }
    MetaClass* clazz = getArrayClass();
    return (clazz == type.clazz || clazz == type.clazz->templateClass);
}
    
list<MetaClass*>& MetaContainer::getClasses()
{
    return classes;
}
    
MetaPackage* MetaContainer::getRootPackage()
{
    return rootPackage;
}

MetaClass* MetaContainer::searchClass(MetaBoxBase* box, const string& name)
{
    //遍历所有父类的兄弟类
    MetaBoxBase* iter = box;
    while (iter && iter->isPackage() == false)
    {
        if (iter->isClass())
        {
            MetaClass* clazz = iter->convertClass();
            if (clazz->name == name)
            {
                return clazz;
            }
            MetaClass* innerClass = clazz->getInnerClass(name);
            if (innerClass)
            {
                return innerClass;
            }
        }
        iter = iter->outer;
    }

    //找同一个文件的类
    MetaFile* file = box->getOuterFile();
    if (file)
    {
        MetaClass* clazz = file->getClass(name);
        if (clazz)
        {
            return clazz;
        }
    }

    //搜索同包下面的所有类
    MetaPackage* package = box->getOuterPackage();
    if (package)
    {
        MetaClass* clazz = package->getClass(name);
        if (clazz)
        {
            return clazz;
        }
    }

    //搜索import里的类
    if (file)
    {
        for (auto& clazz : file->importClasses)
        {
            if (clazz->name == name)
            {
                return clazz;
            }
        }
        for (auto& package : file->importPackages)
        {
            MetaClass* clazz = package->getClass(name);
            if (clazz)
            {
                return clazz;
            }
        }
    }

    return nullptr;
}
    
MetaTemplateParam* MetaContainer::searchTemplateParam(MetaBoxBase* box, const string& name)
{
    //遍历所有父类的兄弟类
    MetaBoxBase* iter = box;
    while (iter && iter->isPackage() == false)
    {
        if (iter->isClass())
        {
            MetaClass* clazz = iter->convertClass();
            MetaTemplateParam* param = clazz->getParam(name);
            if (param)
            {
                return param;
            }
        }
        iter = iter->outer;
    }

    return nullptr;
}

MetaPackage* MetaContainer::searchPackage(MetaBoxBase* box, const string& name)
{
    //搜索本包下面的包名
    MetaPackage* package = box->getOuterPackage();
    MetaPackage* subPackage = package->getPackage(name);
    if (subPackage)
    {
        return subPackage;
    }

    //搜索import
    MetaFile* file = box->getOuterFile();
    if (file)
    {
        for (auto& package : file->importPackages)
        {
            MetaPackage* subPackage = package->getPackage(name);
            if (subPackage)
            {
                return subPackage;
            }
        }
    }

    //搜索顶级包
    package = rootPackage->getPackage(name);
    if (package)
    {
        return package;
    }

    return nullptr;
}
    
MetaFunc* MetaContainer::searchFunction(MetaBoxBase* box, const string& name, list<MetaData>& params, bool onlyStatic, MetaVarRef** varRef)
{
    MetaClass* clazz = box->getOuterClass();
    MetaFunc* func = searchClassFunction(clazz, name, params, onlyStatic);
    if (func)
    {
        return func;
    }

    if (clazz->isAnonyClass)
    {
        //如果是匿名类，先找各上层this
        for (auto& anonyThis : clazz->vars)
        {
            if (anonyThis->varType == VAR_ANONY_THIS)
            {
                func = searchClassFunction(anonyThis->type.clazz, name, params, onlyStatic);
                if (func)
                {
                    *varRef = MetaVarRef::makeVarRef(this, nullptr, anonyThis);
                    return func;
                }
            }
        }
    }
    
    return nullptr;
}
    
MetaFunc* MetaContainer::searchClassFunction(MetaClass* clazz, const string& name, list<MetaData>& params, bool onlyStatic)
{
    int matchValue;
    return searchMatchClassFunction(clazz, name, params, matchValue, onlyStatic);
}
    
MetaFunc* MetaContainer::searchMatchClassFunction(MetaClass* clazz, const string& name, list<MetaData>& params, int& matchValue, bool onlyStatic)
{
    //先查找类型一样的
    MetaFunc* match = nullptr;
    matchValue = 0xFFFF;
    for (auto& func : clazz->funcs)
    {
        if (onlyStatic && func->isStatic == false)
        {
            continue;
        }
        if (func->name != name || func->params.size() != params.size())
        {
            continue;
        }
    
        //比对参数
        int value = 0;
        auto iter1 = func->params.begin();
        auto iter2 = params.begin();
        for (;iter1 != func->params.end();++iter1, ++iter2)
        {
            MetaType type1 = (*iter1)->type;
            MetaType type2 = (*iter2).getType();

            //判断是不是匿名
            if (type1.isClass() && type2.isClass() && type2.clazz->isAnonyClass)
            {
                //匿名类，算匹配
            } 
            else if (type1 != type2)
            {
                if (canAutoConvertType(type2, type1))
                {
                    value++;
                }
                else
                {
                    //不匹配
                    value = 0xFFFF;
                    break;
                }
            }
        }

        if (value < matchValue)
        {
            matchValue = value;
            match = func;
            if (matchValue == 0)
            {
                return match;
            }
        }
    }

    for (auto& parent : clazz->parents)
    {
        int value;
        MetaFunc* func = searchMatchClassFunction(parent, name, params, value, onlyStatic);
        if (value < matchValue)
        {
            matchValue = value;
            match = func;
            if (matchValue == 0)
            {
                return match;
            }
        }
    }

    if (clazz->isAnonyClass)
    {
        //匿名函数，
    }

    return match;
}
    
MetaVarRef* MetaContainer::searchVariable(MetaBoxBase* box, const string& name, bool onlyStatic)
{
    //先找方法再找类
    while (box->isBlock() || box->isFunc())
    {
        MetaVariable* var = box->getVariable(name, onlyStatic);
        if (var)
        {
            return MetaVarRef::makeVarRef(this, nullptr, var);
        }
        box = box->outer;
    }
    if (box->isClass())
    {
        MetaClass* clazz = box->convertClass();
        MetaVariable* var = clazz->getVariable(name, onlyStatic);
        if (var)
        {
            return MetaVarRef::makeVarRef(this, nullptr, var);
        }

        if (clazz->isAnonyClass)
        {
            //如果是匿名类，先找各上层this
            for (auto& anonyThis : clazz->vars)
            {
                if (anonyThis->varType == VAR_ANONY_THIS)
                {
                    MetaVariable* var = anonyThis->type.clazz->getVariable(name, false);
                    if (var)
                    {
                        return MetaVarRef::makeVarRef(this, MetaVarRef::makeVarRef(this, nullptr, anonyThis), var);
                    }
                }
            }

            //继续往上找
            list<MetaClass*> anonyClasses;
            anonyClasses.push_back(clazz);

            box = box->outer;
            while (box)
            {
                MetaVariable* var = box->getVariable(name, false);
                if (var)
                {
                    for (auto& anonyClass : anonyClasses)
                    {
                        //添加变量
                        var = anonyClass->addAnonyMember(var->name, MetaVarRef::makeVarRef(this, nullptr, var));
                    }
                    return MetaVarRef::makeVarRef(this, nullptr, var);
                }

                if (box->isClass())
                {
                    MetaClass* anonyClass = box->convertClass();
                    if (anonyClass->isAnonyClass == false)
                    {
                        break;
                    }
                    anonyClasses.push_front(box->convertClass());
                }
                box = box->outer;
            }
        }
    }
    
    return nullptr;
}
    
bool MetaContainer::canAutoConvertType(const MetaType& src, const MetaType& dst)
{
    if (src.isVoid() && dst.isVoid())
    {
        return true;
    }
    else if (src.isInteger() && dst.isInteger())
    {
        return src.type <= dst.type;
    }
    else if (src.isFloat() && dst.isFloat())
    {
        return src.type <= dst.type;
    }
    else if (src.isClass() && dst.isClass())
    {
        return src.clazz->isBaseOf(dst.clazz);
    }
    else if (src.isNull() && dst.isClass())
    {
        return true;
    }
    return false;
}
   
