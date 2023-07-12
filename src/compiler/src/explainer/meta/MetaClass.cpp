#include "MetaClass.h"
#include "MetaFunc.h"
#include "MetaVariable.h"
#include "MetaBlock.h"
#include "MetaContainer.h"
#include "MetaTemplateParam.h"
#include "MetaFile.h"
#include "MetaVarRef.h"
#include "MetaInstruct.h"
#include "../common/Keyword.h"
#include "../syntax/SyntaxClass.h"
#include "../syntax/SyntaxVarDef.h"
#include "../syntax/SyntaxFunc.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxClassElement.h"
#include "../syntax/SyntaxElement.h"
#include "../syntax/SyntaxSentence.h"
#include "../syntax/SyntaxInstruct.h"
#include "StringUtils.h"
using namespace OneCommon;

MetaClass::MetaClass(const string& name, MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj)
    :MetaBoxBase(BOX_CLASS, outer, metaContainer, syntaxObj)
{
    this->name = name;

    //默认值初始化
    SyntaxClass* syntaxClass = (SyntaxClass*)syntaxObj;
    this->isInterface = syntaxClass->isInterface;
    
    this->this_ = createHideVariable(KEY_THIS, VAR_THIS, false);
    this->this_->type.setClass(this);
    this->class_ = createHideVariable(KEY_CLASS, VAR_CLASS, true);

    if (this->isInterface == false)
    {
        //接口没有super_
        this->super_ = createHideVariable(KEY_SUPER, VAR_SUPER, false);
    }

    this->varInitFunc = createHideFunction(KEY_INIT_VAR_FUNC, FUNC_INIT, false);
    this->staticVarInitFunc = createHideFunction(KEY_INIT_STATIC_VAR_FUNC, FUNC_STATIC_INIT, true);

    for (auto& param : syntaxClass->templateParams)
    {
        addParam(param);
    }

    for (auto& varDef : syntaxClass->vars)
    {
        //将变量初始化提炼varInitFunc和staticVarInitFunc行数
        if (varDef->initExp != nullptr)
        {
            SyntaxExp* exp = varDef->initExp;
            varDef->initExp = nullptr;

            //构建赋值语句
            SyntaxInstruct* instructAssgin = new SyntaxInstruct(syntaxClass->getTempExplainContext());
            instructAssgin->cmd = ASSIGN;
            instructAssgin->params.push_back(varDef);
            instructAssgin->params.push_back(exp->ret);
            exp->instructs.push_back(instructAssgin);
            exp->ret.setInstruct(instructAssgin);

            SyntaxSentence* sentence = new SyntaxSentence(syntaxClass->getTempExplainContext());
            sentence->exp = exp;
            SyntaxElement* element = new SyntaxElement(syntaxClass->getTempExplainContext());
            element->type = SyntaxElement::SENTENCE;
            element->sentence = sentence;

            if (varDef->isStatic)
            {
                ((SyntaxFunc*)this->staticVarInitFunc->syntaxObj)->block->elements.push_back(element);
            }
            else
            {
                ((SyntaxFunc*)this->varInitFunc->syntaxObj)->block->elements.push_back(element);
            }
        }
    }

    //添加到容器
    metaContainer->addClass(this);
}

MetaFunc* MetaClass::createHideFunction(const string& name, int type, bool isStatic)
{
    SyntaxClass* syntaxClass = (SyntaxClass*)this->syntaxObj;

    SyntaxFunc* syntaxFunc = syntaxClass->createFunc(name, type, syntaxClass->getTempExplainContext());
    syntaxFunc->isStatic = isStatic;

    MetaFunc* func = createFunction(name, syntaxFunc);
    func->isHide = true;
    return func;
}
    
MetaVariable* MetaClass::createHideVariable(const string& name, int type, bool isStatic)
{
    SyntaxClass* syntaxClass = (SyntaxClass*)this->syntaxObj;

    SyntaxVarDef* syntaxVar = syntaxClass->createVarDef(name, syntaxClass->getTempExplainContext());

    MetaVariable* var = createVeriable(name, syntaxVar);
    var->varType = type;
    var->isStatic = isStatic;
    var->isConst = true;

    return var;
}

MetaClass* MetaClass::getInnerClass(const string& name)
{
    for (auto& clazz : innerClasses)
    {
        if (clazz->name == name)
        {
            return clazz;
        }
    }
    return nullptr;
}
    
bool MetaClass::isParentClass(MetaClass* parent)
{
    for (auto& parent2 : parents)
    {
        if (parent2 == parent)
        {
            return true;
        }
        if (parent2->isParentClass(parent))
        {
            return true;
        }
    }
    return false;
}
    
bool MetaClass::isBaseOf(MetaClass* parent)
{
    return parent == this || isParentClass(parent);
}
    
MetaClass* MetaClass::getParentClass()
{
    //Object的父类为NULL
    if (super_ == nullptr)
    {
        return nullptr;
    }
    else
    {
        return super_->type.clazz;
    }
}
    
MetaClass* MetaClass::getParentClass(const string& name)
{
    for (auto& parent2 : parents)
    {
        if (parent2->name == name)
        {
            return parent2;
        }
        MetaClass* parent3 = parent2->getParentClass(name);
        if (parent3)
        {
            return parent3;
        }
    }
    return nullptr;
}
    
MetaClass* MetaClass::getParentTamplateClass(MetaClass* clazz)
{
    for (auto& parent2 : parents)
    {
        if (parent2 == clazz || parent2->templateClass == clazz)
        {
            return parent2;
        }
        MetaClass* parent3 = parent2->getParentClass(name);
        if (parent3)
        {
            return parent3;
        }
    }
    return nullptr;
}
    
list<MetaClass*> MetaClass::getParentClasses()
{
    list<MetaClass*> result;
    result = parents;
    for (auto& parent : parents)
    {
        auto result2 = parent->getParentClasses();
        result.splice(result.end(), result2);
    }
    return result;
}
    
MetaVariable* MetaClass::createVeriable(const string& name, SyntaxBase* syntaxObj)
{
    return new MetaVariable(name, VAR_MEMBER, this, metaContainer, syntaxObj);
}
    
MetaFunc* MetaClass::createFunction(const string& name, SyntaxBase* syntaxObj)
{
    return new MetaFunc(name, this, metaContainer, syntaxObj);
}
    
MetaClass* MetaClass::addInnerClass(const string& name, SyntaxBase* syntaxObj)
{
    MetaClass* clazz = new MetaClass(name, this, metaContainer, syntaxObj);
    clazz->isInnerClass = true;
    innerClasses.push_back(clazz);
    return clazz;
}

MetaFunc* MetaClass::addFunction(const string& name, SyntaxBase* syntaxObj)
{
    MetaFunc* func = createFunction(name, syntaxObj);
    funcs.push_back(func);
    return func;
}
	
MetaFunc* MetaClass::addFunction(const string& name, int funcType)
{
    MetaFunc* func = createFunction(name, nullptr);
    func->funcType = funcType;
    funcs.push_back(func);
    return func;
}
    
MetaVariable* MetaClass::addVeriable(const string& name, SyntaxBase* syntaxObj)
{
    MetaVariable* var = createVeriable(name, syntaxObj);
    vars.push_back(var);
    return var;
}
    
MetaVariable* MetaClass::addVeriable(const string& name, MetaType& type)
{
    MetaVariable* var = createVeriable(name, nullptr);
    var->type = type;
    vars.push_back(var);
    return var;
}
    
MetaTemplateParam* MetaClass::addParam(const string& name)
{
    MetaTemplateParam* param = new MetaTemplateParam(name, metaContainer);
    params.push_back(param);
    return param;
}
    
MetaTemplateParam* MetaClass::getParam(const string& name)
{
    for (auto& param : params)
    {
        if (param->name == name)
        {
            return param;
        }
    }
    return nullptr;
}
    
bool MetaClass::isRealClass()
{
    // for (auto& param : params)
    // {
    //     if (param->haveType() == false)
    //     {
    //         return false;
    //     }
    // }
    // return true;
    return params.empty() || templateClass != nullptr;
}
    
bool MetaClass::isTemplateRealClass()
{
    //return params.empty() == false && isRealClass();
    return templateClass != nullptr;
}
    
bool MetaClass::isNormalRealClass()
{
    return params.empty();
}
    
bool MetaClass::isTemplateClass()
{
    if (params.empty() == false && templateClass == nullptr)
    {
        //是模板类
        return true;
    }
    else
    {
        //外部类也不能是模板类
        return outer && outer->getOuterClass() && outer->getOuterClass()->isTemplateClass();
    }
}
    
int MetaClass::getTemplateNestNum()
{
    int maxNum = 1;
    for (auto& param : params)
    {
        if (param->type.isClass())
        {
            int num = param->type.clazz->getTemplateNestNum() + 1;
            maxNum = std::max(maxNum, num);
        }
    }
    return maxNum;
}
    
MetaClass* MetaClass::createRealClass(const list<MetaType>& types)
{
    if (isRealClass())
    {
        return nullptr;
    }
    if (params.size() != types.size())
    {
        return nullptr;
    }

    // 查找是否已经存在
    for (auto& realClass : realClasses)
    {
        bool sameTypes = true;
        
        auto iter = types.begin();
        for (auto& param : realClass->params)
        {
            //如果对应的模板不一样，
            if (param->type != *iter)
            {
                sameTypes = false;
                break;
            }
        }

        if (sameTypes)
        {
            return realClass;
        }
    }

    MetaClass* realClazz = nullptr;
    if (outer->boxType == MetaBoxBase::BOX_CLASS)
    {
        realClazz = ((MetaClass*)outer)->addInnerClass(this->name, this->syntaxObj);
    }
    else if (outer->boxType == MetaBoxBase::BOX_FILE)
    {
        realClazz = ((MetaFile*)outer)->addClass(this->name, this->syntaxObj);
    }
    realClazz->templateClass = this;

    auto iter = types.begin();
    for (auto& param : realClazz->params)
    {
        param->type = *iter;
        ++iter;
    }
    
    realClasses.push_back(realClazz);
    return realClazz;
}
    
MetaVariable* MetaClass::getThisVariable()
{
    return this_;
}
    
MetaVariable* MetaClass::getSuperVariable()
{
    return super_;
}

MetaVariable* MetaClass::getVariable(const string& name, int filterType)
{
    if (filterType & MFT_ONLY_STATIC)
    {
        if (class_->name == name)
        {
            return class_;
        }
    }
    if (filterType & MFT_ONLY_NORMAL)
    {
        if (this_->name == name)
        {
            return this_;
        }
        if (super_ && super_->name == name)
        {
            return super_;
        }
    }

    for (auto& var : vars)
    {
        if (var->name == name && filterMember(var->isStatic, filterType))
        {
            return var;
        }
    }

    for (auto& parent : parents)
    {
        MetaVariable* var = parent->getVariable(name, filterType);
        if (var)
        {
            return var;
        }
    }

    return nullptr;
}
    
MetaFunc* MetaClass::getFunction(const string& name, int filterType)
{
    for (auto& func : funcs)
    {
        if (func->name == name && filterMember(func->isStatic, filterType))
        {
            return func;
        }
    }
    return nullptr;
}
    
Result MetaClass::verifyAndRepair()
{
    SyntaxClass* syntaxClass = (SyntaxClass*)syntaxObj;
    if (isInterface)
    {
        for (auto& parent : parents)
        {
            if (parent->isInterface == false)
            {
                //父类必须是interface
                return R_FAILED;
            }
        }
        if (parents.empty() && metaContainer->getInterfaceClass() != this)
        {
            //确保接口从Interface继承
            parents.push_front(metaContainer->getInterfaceClass());
        }

        if (innerClasses.empty() == false)
        {
            //不能有内嵌类
            return R_FAILED;
        }

        for (auto& func : funcs)
        {
            if (func->isVirtual == false || func->isStatic)
            {
                //不能包含非虚函数和静态函数
                return R_FAILED;
            }
        }

        if (vars.empty() == false)
        {
            //不能有变量
            return R_FAILED;
        }
    }
    else
    {
        int num = 0;
        for (auto& parent : parents)
        {
            if (parent->isInterface == false)
            {
                num++;
            }
        }
        if (num > 1)
        {
            //单继承
            return R_FAILED;
        }

        if (num > 0)
        {
            //第一个是父类
            super_->type.setClass(parents.front());
        }
        else
        {
            //
            super_->type.setClass(metaContainer->getObjectClass());
        }

        // 看有没有构造函数和析构函数，不然补充一个空的
        bool haveConstruct = false;
        bool haveDestruct = false;
        for (auto& func : funcs)
        {
            if (func->funcType == FUNC_CONSTRUCT)
            {
                if (func->name != name)
                {
                    return R_FAILED;
                }
                haveConstruct = true;
            }
            else if (func->funcType == FUNC_DESTRUCT)
            {
                if (func->name != "~" + name)
                {
                    return R_FAILED;
                }
                haveDestruct = true;
            }
        }

        if (haveConstruct == false)
        {
            //添加默认构造函数
            MetaFunc* func = addFunction(name, syntaxClass->createFunc(name, FUNC_CONSTRUCT, syntaxClass->getTempExplainContext()));
            //TODO: func->returnType.setClass(this);
        }
        if (haveDestruct == false)
        {
            MetaFunc* func = addFunction("~" + name, syntaxClass->createFunc("~" + name, FUNC_DESTRUCT, syntaxClass->getTempExplainContext()));
            func->isVirtual = true;
        }
    }

    //完善类型
    this->class_->type.setClass(metaContainer->getClassClass());

    return {};
}
    
void MetaClass::addLinkClass(MetaClass* clazz)
{
    if (clazz != this && clazz->isRealClass())
    {
        if (std::find(linkClasses.begin(), linkClasses.end(), clazz) == linkClasses.end())
        {
            linkClasses.push_back(clazz);
        }
    }
}
    
list<MetaVariable*> MetaClass::getAnonyThises()
{
    list<MetaVariable*> vars;
    for (auto& var : vars)
    {
        if (var->varType == VAR_ANONY_THIS)
        {
            vars.push_back(var);
        }
    }
    return vars;
}
    
MetaVariable* MetaClass::addAnonyThis(int index, MetaVarRef* varRef)
{
    string name = "this" + StringUtils::itoa(index);
    MetaVariable* var = addAnonyMember(name, varRef);
    var->varType = VAR_ANONY_THIS;
    var->isConst = true;
    return var;
}
    
MetaVariable* MetaClass::addAnonyMember(const string& name, MetaVarRef* varRef)
{
    //构造函数增加一个参数
    string varName = "var" + StringUtils::itoa((int)anonyConstructFunc->params.size());
    MetaVariable* param = anonyConstructFunc->addParam(varName, varRef->type);

    //增加一个成员变量，放传进来的参数
    MetaVariable* member = this->addVeriable(name, varRef->type);

    //构造函数增加一条语句，把参数赋值给成员变量
    MetaInstruct* assign = new MetaInstruct(metaContainer, nullptr);
    assign->cmd = ASSIGN;
    assign->params.push_back(member);
    assign->params.push_back(param);
    assign->retType = member->type;
    anonyConstructFunc->block->addInstruct(assign);

    //new语句增加各参数，把变量传给参数
    newAnonyInstruct->params.push_back(varRef);

    return member;
}
    
bool MetaClass::isFunctionClass()
{
    return isInterface && funcs.size() == 1 && funcs.front()->isVirtual;
}
    
MetaFunc* MetaClass::getFunctionClassBody()
{
    if (isFunctionClass() == false)
    {
        return nullptr;
    }
    return funcs.front();
}

