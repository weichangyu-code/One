#pragma once
#include "MetaBoxBase.h"
#include "MetaType.h"
#include "../common/FuncType.h"

class MetaVariable;
class MetaClass;
class MetaConst;
class MetaFunc : public MetaBoxBase
{
public:
    MetaFunc(const string& name, MetaClass* clazz, MetaContainer* metaContainer, SyntaxBase* syntaxObj);

    MetaVariable* addParam(const string& name, SyntaxBase* syntaxObj);
    MetaVariable* addParam(const string& name, MetaType& type);
    
    MetaClass* addAnonyClass(MetaBoxBase* box, SyntaxBase* syntaxObj);

    MetaVariable* getVariable(const string& name, bool onlyStatic);

    bool isInitVarFunc();
    
public:
    int funcType = FUNC_NORMAL;

    string name;
    bool isStatic = false;
    bool isVirtual = false;
    bool isHidden = false;             //隐藏函数

    list<MetaVariable*> params;
    MetaType return_;

    list<MetaConst*> defualtParams;     //TODO: 默认值，默认值必须是常量

    MetaBlock* block = nullptr;

    //匿名类
    list<MetaClass*> anonyClasses;
};
