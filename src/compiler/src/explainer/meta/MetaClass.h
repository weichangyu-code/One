#pragma once
#include "MetaBoxBase.h"
#include "../../common/Result.h"
#include "MetaType.h"
#include "../common/Member.h"

class MetaPackage;
class MetaFunc;
class MetaVariable;
class MetaFile;
class MetaBlock;
class MetaTemplateParam;
class MetaVarRef;
class MetaInstruct;
class MetaClass : public MetaBoxBase
{
public:
    MetaClass(const string& name, MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj);

    MetaClass* getInnerClass(const string& name);

    bool isParentClass(MetaClass* parent);
    bool isBaseOf(MetaClass* parent);
    MetaClass* getParentClass();
    MetaClass* getParentClass(const string& name);
    MetaClass* getParentTamplateClass(MetaClass* clazz);
    list<MetaClass*> getParentClasses();

    MetaVariable*   createVeriable(const string& name, SyntaxBase* syntaxObj);
    MetaFunc*       createFunction(const string& name, SyntaxBase* syntaxObj);

    MetaClass*      addInnerClass(const string& name, SyntaxBase* syntaxObj);
	MetaFunc*       addFunction(const string& name, SyntaxBase* syntaxObj);
	MetaFunc*       addFunction(const string& name, int funcType);
    MetaVariable*   addVeriable(const string& name, SyntaxBase* syntaxObj);
    MetaVariable*   addVeriable(const string& name, MetaType& type);

    MetaTemplateParam* addParam(const string& name);
    MetaTemplateParam* getParam(const string& name);
    bool            isRealClass();                              //可以实例化的类
    bool            isTemplateRealClass();                      //模板实例化的类
    bool            isNormalRealClass();                        //普通实体类
    bool            isTemplateClass();                          //纯模板类，不能实例化
    MetaClass*      createRealClass(const list<MetaType>& types);

    MetaVariable* getVariable(const string& name, int filterType);
    MetaVariable* getThisVariable();
    MetaVariable* getSuperVariable();

    MetaFunc* getFunction(const string& name, int filterType);

    Result verifyAndRepair();

    void addLinkClass(MetaClass* clazz);

    //匿名类的外层this
    list<MetaVariable*> getAnonyThises();
    MetaVariable* addAnonyThis(int index, MetaVarRef* varRef);
    MetaVariable* addAnonyMember(const string name, MetaVarRef* varRef);

public:
    int id = 0;                         //所以Class的索引，从1开始
    string name;
    bool isInterface = false;
    bool isInnerClass = false;
    bool isAnonyClass = false;

    enum
    {
        STEP_READY,
        STEP_DECLARE,
        STEP_STRUCT,
        STEP_INSTRUCT,
    };
    int step = STEP_READY;

    list<MetaTemplateParam*> params;
    list<MetaClass*> realClasses;           //所有实例化的类
    MetaClass* templateClass = nullptr;     //realClass对应的模板类

    list<MetaClass*> parents;
    list<MetaClass*> innerClasses;

    MetaVariable* this_ = nullptr;
    MetaVariable* super_ = nullptr;
    MetaVariable* class_ = nullptr;
    list<MetaFunc*> funcs;
    list<MetaVariable*> vars;

    MetaFunc* varInitFunc = nullptr;
    MetaFunc* staticVarInitFunc = nullptr;
    
    list<MetaClass*> linkClasses;

    //匿名类相关的参数
    MetaInstruct* newAnonyInstruct = nullptr;         //new匿名类的指令
    MetaFunc* anonyConstructFunc = nullptr;
};