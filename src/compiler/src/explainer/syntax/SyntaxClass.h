#pragma once
#include "SyntaxBase.h"
#include "SyntaxMulti.h"
#include "../common/Keyword.h"

class SyntaxType;
class SyntaxVarDef;
class SyntaxFunc;
class SyntaxBlock;
class SyntaxFile;
class SyntaxClassElement;
class SyntaxClass : public SyntaxBase
{
public:
    SyntaxClass(ExplainContext* context);

public:
    void addInnerClass(SyntaxClass* clazz);

    SyntaxFunc* createFunc(const string& name, int type);
    SyntaxVarDef* createVarDef(const string& name);
    void addElements(ExplainContext* context, SyntaxMulti<SyntaxClassElement*>* elements);

public:
    SyntaxFile* file = nullptr;
    SyntaxClass* outerClass = nullptr;
    string name;

    bool isInterface = false;
    bool isAnonyClass = false;

    //模板参数
    list<string> templateParams;

    //父类
    list<SyntaxType*> parents;

    //成员变量
    list<SyntaxVarDef*> vars;

    //成员方法
    list<SyntaxFunc*> funcs;

    //内嵌类
    list<SyntaxClass*> innerClasses;

    //变量初始化
    SyntaxFunc* varInitFunc = nullptr;
    SyntaxFunc* staticVarInitFunc = nullptr;

    //this
    SyntaxVarDef* this_ = nullptr;
    SyntaxVarDef* super_ = nullptr;
    SyntaxVarDef* class_ = nullptr;
};
