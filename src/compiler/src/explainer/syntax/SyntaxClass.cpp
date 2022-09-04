#include "SyntaxClass.h"
#include "SyntaxBlock.h"
#include "SyntaxFunc.h"
#include "SyntaxVarDef.h"
#include "SyntaxClassElement.h"
#include "SyntaxElement.h"
#include "SyntaxSentence.h"
#include "SyntaxExp.h"
#include "../explain/ExplainContext.h"
#include "../common/Keyword.h"

SyntaxClass::SyntaxClass(ExplainContext* context)
    :SyntaxBase(context)
{
    this->file = context->file;

    this->varInitFunc = createFunc(KEY_INIT_VAR_FUNC, FUNC_INIT);

    this->staticVarInitFunc = createFunc(KEY_INIT_STATIC_VAR_FUNC, FUNC_STATIC_INIT);
    this->staticVarInitFunc->isStatic = true;

    this->this_ = createVarDef(KEY_THIS);
    this->super_ = createVarDef(KEY_SUPER);
}

void SyntaxClass::addInnerClass(SyntaxClass* clazz)
{
    clazz->outerClass = this;
    innerClasses.push_back(clazz);
}
    
SyntaxFunc* SyntaxClass::createFunc(const string& name, int type)
{
    ExplainContext context(file);
    SyntaxFunc* func = new SyntaxFunc(&context);
    func->name = name;
    func->type = type;
    func->block = new SyntaxBlock(&context);
    return func;
}
    
SyntaxVarDef* SyntaxClass::createVarDef(const string& name)
{
    ExplainContext context(file);
    SyntaxVarDef* varDef = new SyntaxVarDef(&context);
    varDef->name = name;
    return varDef;
}
    
void SyntaxClass::addElements(ExplainContext* context, SyntaxMulti<SyntaxClassElement*>* elements)
{
    for (auto& element : elements->items)
    {
        if (element->varDef)
        {
            SyntaxVarDef* varDef = element->varDef;

            //添加变量
            this->vars.push_back(varDef);

            //添加初始化代码块
            if (varDef->isConst == false && varDef->exp != nullptr && varDef->exp->instructs.empty() == false)
            {
                varDef->addAssginInstruct(context);

                SyntaxSentence* sentence = new SyntaxSentence(context);
                sentence->exp = varDef->exp;
                SyntaxElement* element = new SyntaxElement(context);
                element->type = SyntaxElement::SENTENCE;
                element->sentence = sentence;
                if (varDef->isStatic)
                {
                    this->staticVarInitFunc->block->elements.push_back(element);
                }
                else
                {
                    this->varInitFunc->block->elements.push_back(element);
                }
            }
        }
        else if (element->func)
        {        
            SyntaxFunc* func = element->func;

            //添加方法
            this->funcs.push_back(func);
        }
        else if (element->clazz)
        {
            SyntaxClass* innerClass = element->clazz;

            //添加内嵌
            this->addInnerClass(innerClass);
        }
    }
}

