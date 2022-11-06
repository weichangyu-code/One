#include "MetaContainer.h"
#include "MetaPackage.h"
#include "StringUtils.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "MetaFunc.h"
#include "MetaFile.h"
#include "MetaVariable.h"
#include "MetaVarRef.h"
#include "MetaConst.h"
#include "MetaTemplateParam.h"
#include "../common/Keyword.h"
using namespace OneCommon;

MetaContainer::MetaContainer()
{
    rootPackage = new MetaPackage("", nullptr, this);

    addDefaultAutoConvert();
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

MetaClass* MetaContainer::getClassClass()
{
    if (classClass == nullptr)
    {
        classClass = getClass(KEY_ONE_CLASS_CLASS);
    }
    return classClass;
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
    
void MetaContainer::addClass(MetaClass* clazz)
{
    clazz->id = (int)classes.size() + 1;
    classes.push_back(clazz);
}
    
const list<MetaClass*>& MetaContainer::getClasses()
{
    return classes;
}
    
MetaPackage* MetaContainer::getRootPackage()
{
    return rootPackage;
}
    
MetaConst* MetaContainer::getZeroConst()
{
    if (zeroConst == nullptr)
    {
        zeroConst = new MetaConst(this, nullptr);
        zeroConst->setIntValue(0);
        zeroConst->remark = "0";
    }
    return zeroConst;
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
    
MetaFunc* MetaContainer::searchFunction(MetaBoxBase* box, const string& name, const list<MetaData>& params, int filterType, MetaVarRef** varRef)
{
    MetaClass* clazz = box->getOuterClass();
    MetaFunc* func = searchClassFunction(clazz, name, params, filterType);
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
                func = searchClassFunction(anonyThis->type.clazz, name, params, filterType);
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
    
MetaFunc* MetaContainer::searchClassFunction(MetaClass* clazz, const string& name, const list<MetaData>& params, int filterType)
{
    int matchValue;
    return searchMatchClassFunction(clazz, name, params, matchValue, filterType);
}
    
MetaFunc* MetaContainer::searchMatchClassFunction(MetaClass* clazz, const string& name, const list<MetaData>& params, int& matchValue, int filterType)
{
    //先查找类型一样的
    MetaFunc* match = nullptr;
    matchValue = 0xFFFF;
    for (auto& func : clazz->funcs)
    {
        if (filterMember(func->isStatic, filterType) == false)
        {
            continue;
        }
        if (func->name != name)
        {
            //会有默认参数，所以函数参数数量大于等于实际数量
            continue;
        }

        int value = 0;
        //判断是否有可变参数
        if (func->isDynamicParamFunc())
        {
            if (func->params.size() - 1 > params.size())
            {
                //除了可变参数，其他参数必填
                continue;
            }

            //比对参数
            auto iter1 = func->params.begin();
            auto iter2 = params.begin();
            for (;iter1 != func->params.end();++iter1, ++iter2)
            {
                if ((*iter1)->isDynamic)
                {
                    break;
                }

                MetaType type1 = (*iter1)->type;
                MetaType type2 = (*iter2).getType();

                //判断是不是匿名
                if (type1.isClass() && type2.isClass() && type2.clazz->isAnonyClass)
                {
                    //匿名类，算匹配
                } 
                else if (type1 != type2)
                {
                    if (getAutoConvertType(type2, type1) != ACT_CANNT)
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

            //判断多余的参数有没有默认参数
            MetaType type1 = func->getDynamicParamType();    //数组里面的第一个模板参数
            for (;iter2 != params.end();++iter2)
            {
                MetaType type2 = (*iter2).getType();

                //判断是不是匿名
                if (type1.isClass() && type2.isClass() && type2.clazz->isAnonyClass)
                {
                    //匿名类，算匹配
                } 
                else if (type1 != type2)
                {
                    if (getAutoConvertType(type2, type1) != ACT_CANNT)
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
        }
        else
        {
            if (func->params.size() < params.size())
            {
                //会有默认参数，所以函数参数数量大于等于实际数量
                continue;
            }

            //比对参数
            auto iter1 = func->params.begin();
            auto iter2 = params.begin();
            for (;iter2 != params.end();++iter1, ++iter2)
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
                    if (getAutoConvertType(type2, type1) != ACT_CANNT)
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

            //判断多余的参数有没有默认参数
            for (;iter1 != func->params.end();++iter1)
            {
                MetaVariable* var = *iter1;
                if (var->initBlock == nullptr)
                {
                    //不是默认参数，不匹配
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
        MetaFunc* func = searchMatchClassFunction(parent, name, params, value, filterType);
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

    return match;
}
    
MetaVarRef* MetaContainer::searchVariable(MetaBoxBase* box, const string& name, int filterType)
{
    //先找方法再找类
    while (box->isBlock() || box->isFunc())
    {
        MetaVariable* var = box->getVariable(name, MFT_ALL);
        if (var)
        {
            return MetaVarRef::makeVarRef(this, nullptr, var);
        }
        box = box->outer;
    }
    if (box->isClass())
    {
        MetaClass* clazz = box->convertClass();
        MetaVariable* var = clazz->getVariable(name, filterType);
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
                    MetaVariable* var = anonyThis->type.clazz->getVariable(name, MFT_ALL);
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
                MetaVariable* var = box->getVariable(name, MFT_ALL);
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
    
// bool MetaContainer::canAutoConvertType(const MetaType& src, const MetaType& dst)
// {
//     if (src.isVoid() && dst.isVoid())
//     {
//         return true;
//     }
//     else if (src.isBool() && dst.isBool())
//     {
//         return src.type <= dst.type;
//     }
//     else if (src.isInteger() && dst.isInteger())
//     {
//         return src.type <= dst.type;
//     }
//     else if (src.isFloat() && dst.isFloat())
//     {
//         return src.type <= dst.type;
//     }
//     else if (src.isClass() && dst.isClass())
//     {
//         return src.clazz->isBaseOf(dst.clazz);
//     }
//     else if (src.isNull() && dst.isClass())
//     {
//         return true;
//     }
//     return false;
// }
    
void MetaContainer::addDefaultAutoConvert()
{
    addAutoConvertType(DT_CHAR, DT_SHORT, ACT_BASE_TYPE);
    addAutoConvertType(DT_CHAR, DT_INT, ACT_BASE_TYPE);
    addAutoConvertType(DT_CHAR, DT_LONG, ACT_BASE_TYPE);
    addAutoConvertType(DT_SHORT, DT_INT, ACT_BASE_TYPE);
    addAutoConvertType(DT_SHORT, DT_LONG, ACT_BASE_TYPE);
    addAutoConvertType(DT_INT, DT_LONG, ACT_BASE_TYPE);
    addAutoConvertType(DT_FLOAT, DT_DOUBLE, ACT_BASE_TYPE);
}
    
void MetaContainer::addAutoConvertType(const MetaType& src, const MetaType& dst, int type)
{
    autoConvertData[src][dst] = type;
}
    
int  MetaContainer::getAutoConvertType(const MetaType& src, const MetaType& dst)
{
    if (src == dst)
    {
        return ACT_EQUAL;
    }
    else if (src.isClass() && dst.isClass())
    {
        if (src.clazz->isBaseOf(dst.clazz))
        {
            //子类可以转换成子类
            return ACT_PARENT_TYPE;
        }
        else if (src.clazz->templateClass != nullptr && src.clazz->templateClass == dst.clazz->templateClass)
        {
            //如果是同一个模板继承来的，判断模板
            bool canConvert = true;
            auto paramIterSrc = src.clazz->params.begin();
            auto paramIterDst = dst.clazz->params.begin();
            for (;paramIterSrc != src.clazz->params.end();++paramIterSrc, ++paramIterDst)
            {
                auto typeSrc = (*paramIterSrc)->type;
                auto typeDst = (*paramIterDst)->type;
                if (typeSrc == typeDst)
                {

                }
                else if (typeSrc.isClass() && typeDst.isClass() 
                    && typeSrc.clazz->isBaseOf(typeDst.clazz) && typeDst.clazz->isInterface == false)
                {
                    //继承关系，并且转换不能是接口，因为接口和对象的指针偏移不一样
                }
                else
                {
                    canConvert = false;
                    break;
                }
            }
            if (canConvert)
            {
                return ACT_TEMPLATE;
            }
        }
    }
    else if (src.isNull() && dst.isClass())
    {
        //null可以转换任意类
        return ACT_NULL;
    }

    auto iter1 = autoConvertData.find(src);
    if (iter1 == autoConvertData.end())
    {
        return ACT_CANNT;
    }
    auto iter2 = iter1->second.find(dst);
    if (iter2 == iter1->second.end())
    {
        return ACT_CANNT;
    }
    return iter2->second;
}
   
