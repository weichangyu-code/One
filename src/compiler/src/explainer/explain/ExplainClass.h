#pragma once
#include "ExplainBase.h"
#include "../syntax/SyntaxClass.h"
#include "../syntax/SyntaxFile.h"
#include "../syntax/SyntaxVarDef.h"
#include "../syntax/SyntaxMulti.h"
#include "../syntax/SyntaxFunc.h"
#include "../syntax/SyntaxElement.h"
#include "../syntax/SyntaxSentence.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxClassElement.h"

class ExplainClass : public ExplainBase
{
public:
    ExplainClass(ExplainContainer* container, ExplainContext* context)
        : ExplainBase(container, context)
    {
        registe("templatedef", "", (MyRuleExecuteFunction)&onExplainTemplateDef);

        registe("classidentifier", "identifier", (MyRuleExecuteFunction)&onExplainClassIdentifier);
        registe("classidentifier", "templatedef", (MyRuleExecuteFunction)&onExplainClassIdentifierTemplate);
        registe("classheaderstart", "class", (MyRuleExecuteFunction)&onExplainClassHeaderStartClass);
        registe("classheaderstart", "interface", (MyRuleExecuteFunction)&onExplainClassHeaderStartInterface);
        registe("classparents", "", (MyRuleExecuteFunction)&onExplainClassParents);
        registe("classparents", "add", (MyRuleExecuteFunction)&onExplainClassParentsAdd);
        registe("classheader", "parents", (MyRuleExecuteFunction)&onExplainClassHeaderParents);
        registe("classheader", "", (MyRuleExecuteFunction)&onExplainClassHeader);
        registe("classdef", "", (MyRuleExecuteFunction)&onExplainClassDef);
        
        registe("classelement", "vardeftype", (MyRuleExecuteFunction)&onExplainClassElementVarDef);
        registe("classelement", "funcdef", (MyRuleExecuteFunction)&onExplainClassElementFuncDef);
        registe("classelement", "classdef", (MyRuleExecuteFunction)&onExplainClassElementClassDef);
        registe("classelement", "constructdef", (MyRuleExecuteFunction)&onExplainClassElementConstructDef);
        registe("classelement", "destructdef", (MyRuleExecuteFunction)&onExplainClassElementDestructDef);

        registe("classelements", "", (MyRuleExecuteFunction)&onExplainClassElements);
        registe("classelements", "add", (MyRuleExecuteFunction)&onExplainClassElementsAdd);
        registe("classbody", "", (MyRuleExecuteFunction)&onExplainClassBody);
        registe("classbody", "null", (MyRuleExecuteFunction)&onExplainClassBodyNull);
    }

public:
    Result onExplainTemplateDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        out.ptr = es[1].ptr;
        return {};
    }

    Result onExplainClassIdentifier(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = new SyntaxClass(context);
        clazz->name = es[0].remark;

        //
        out.ptr = clazz;
        return {};
    }

    Result onExplainClassIdentifierTemplate(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        VR(onExplainClassIdentifier(rule, es, out));

        SyntaxClass* clazz = (SyntaxClass*)out.ptr;
        SyntaxMulti<string>* mi = (SyntaxMulti<string>*)es[1].ptr;

        clazz->templateParams = std::move(mi->items);

        return {};
    }

    Result onExplainClassHeaderStartClass(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = (SyntaxClass*)es[1].ptr;
        out.ptr = clazz;
        return {};
    }

    Result onExplainClassHeaderStartInterface(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = (SyntaxClass*)es[1].ptr;
        clazz->isInterface = true;
        out.ptr = clazz;
        return {};
    }

    Result onExplainClassParents(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* parents = new SyntaxMulti<SyntaxType*>(context);
        parents->items.push_back((SyntaxType*)es[0].ptr);
        out.ptr = parents;
        return {};
    }

    Result onExplainClassParentsAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxType*>* parents = (SyntaxMulti<SyntaxType*>*)es[0].ptr;
        parents->items.push_back((SyntaxType*)es[2].ptr);
        out.ptr = parents;
        return {};
    }

    Result onExplainClassHeaderParents(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = (SyntaxClass*)es[0].ptr;
        SyntaxMulti<SyntaxType*>* parents = (SyntaxMulti<SyntaxType*>*)es[2].ptr;
        clazz->parents = std::move(parents->items);
        out.ptr = parents;
        return {};
    }

    Result onExplainClassHeader(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        return {};
    }

    Result onExplainClassDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClass* clazz = (SyntaxClass*)es[0].ptr;
        SyntaxMulti<SyntaxClassElement*>* multi = (SyntaxMulti<SyntaxClassElement*>*)es[1].ptr;

        clazz->addElements(context, multi);

        out.ptr = clazz;
        return {};
    }

    Result onExplainClassElementVarDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClassElement* element = new SyntaxClassElement(context);
        element->varDef = (SyntaxVarDef*)es[0].ptr;

        out.ptr = element;
        return {};
    }
    
    Result onExplainClassElementFuncDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClassElement* element = new SyntaxClassElement(context);
        element->func = (SyntaxFunc*)es[0].ptr;

        out.ptr = element;
        return {};
    }
    
    Result onExplainClassElementClassDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClassElement* element = new SyntaxClassElement(context);
        element->clazz = (SyntaxClass*)es[0].ptr;

        out.ptr = element;
        return {};
    }
    
    Result onExplainClassElementConstructDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClassElement* element = new SyntaxClassElement(context);
        element->func = (SyntaxFunc*)es[0].ptr;

        out.ptr = element;
        return {};
    }
    
    Result onExplainClassElementDestructDef(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxClassElement* element = new SyntaxClassElement(context);
        element->func = (SyntaxFunc*)es[0].ptr;

        out.ptr = element;
        return {};
    }
    
    Result onExplainClassElements(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxClassElement*>* multi = new SyntaxMulti<SyntaxClassElement*>(context);
        multi->items.push_back((SyntaxClassElement*)es[0].ptr);

        out.ptr = multi;
        return {};
    }
    
    Result onExplainClassElementsAdd(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxClassElement*>* multi = (SyntaxMulti<SyntaxClassElement*>*)es[0].ptr;
        multi->items.push_back((SyntaxClassElement*)es[1].ptr);

        out.ptr = multi;
        return {};
    }
    
    Result onExplainClassBody(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxClassElement*>* multi = (SyntaxMulti<SyntaxClassElement*>*)es[1].ptr;
        out.ptr = multi;
        return {};
    }

    Result onExplainClassBodyNull(Rule* rule, vector<LexElement>& es, LexElement& out)
    {
        SyntaxMulti<SyntaxClassElement*>* multi = new SyntaxMulti<SyntaxClassElement*>(context);
        out.ptr = multi;
        return {};
    }
};
