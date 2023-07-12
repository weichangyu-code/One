#include "SyntaxClass.h"
#include "SyntaxBlock.h"
#include "SyntaxFunc.h"
#include "SyntaxVarDef.h"
#include "SyntaxClassElement.h"
#include "SyntaxElement.h"
#include "SyntaxSentence.h"
#include "SyntaxExp.h"
#include "SyntaxFile.h"
#include "../explain/ExplainContext.h"
#include "../common/Keyword.h"

SyntaxClass::SyntaxClass(ExplainContext* context)
    :SyntaxBase(context)
{
    this->file = context->file;
}

void SyntaxClass::addInnerClass(SyntaxClass* clazz)
{
    clazz->outerClass = this;
    innerClasses.push_back(clazz);
}
    
SyntaxFunc* SyntaxClass::createFunc(const string& name, int type, ExplainContext* context)
{
    SyntaxFunc* func = new SyntaxFunc(context);
    func->name = name;
    func->type = type;
    func->block = new SyntaxBlock(context);
    return func;
}
    
SyntaxVarDef* SyntaxClass::createVarDef(const string& name, ExplainContext* context)
{
    SyntaxVarDef* varDef = new SyntaxVarDef(context);
    varDef->name = name;
    return varDef;
}
    
void SyntaxClass::addElements(SyntaxMulti<SyntaxClassElement*>* elements)
{
    for (auto& element : elements->items)
    {
        if (element->varDef)
        {
            SyntaxVarDef* varDef = element->varDef;

            //添加变量
            this->vars.push_back(varDef);
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
    
ExplainContext* SyntaxClass::getTempExplainContext()
{
    return file->getTempExplainContext();
}

