#include "MetaGenerator.h"
#include "MetaPackage.h"
#include "MetaFile.h"
#include "MetaClass.h"
#include "MetaFunc.h"
#include "MetaVariable.h"
#include "MetaBlock.h"
#include "MetaConst.h"
#include "MetaData.h"
#include "MetaVariable.h"
#include "MetaVarRef.h"
#include "MetaInstruct.h"
#include "MetaTemplateParam.h"
#include "../syntax/SyntaxFile.h"
#include "../syntax/SyntaxClass.h"
#include "../syntax/SyntaxVarDef.h"
#include "../syntax/SyntaxType.h"
#include "../syntax/SyntaxFunc.h"
#include "../syntax/SyntaxElement.h"
#include "../syntax/SyntaxSentence.h"
#include "../syntax/SyntaxExp.h"
#include "../syntax/SyntaxConst.h"
#include "../syntax/SyntaxInstruct.h"
#include "../syntax/SyntaxVar.h"
#include "../syntax/SyntaxBlock.h"
#include "../syntax/SyntaxIfBlock.h"
#include "../syntax/SyntaxWhileBlock.h"
#include "../syntax/SyntaxForBlock.h"
#include "StringUtils.h"
#include "../common/Keyword.h"
using namespace OneCommon;

MetaGenerator::MetaGenerator(MetaContainer* metaContainer)
{
    this->metaContainer = metaContainer;
}

Result MetaGenerator::addSyntaxFile(MetaPackage* package, const string& name, SyntaxFile* syntaxFile)
{
    package->addFile(name, syntaxFile);
    return {};
}
    
Result MetaGenerator::generate()
{
    VR(generateMetaPackageDeclare(metaContainer->getRootPackage()));
    VR(generateMetaPackageStruct(metaContainer->getRootPackage()));
    VR(generateMetaPackageInstruct(metaContainer->getRootPackage()));
    return {};
}
    
Result MetaGenerator::generateMetaPackageDeclare(MetaPackage* package)
{
    for (auto& sub : package->packages)
    {
        VR(generateMetaPackageDeclare(sub));
    }
    for (auto& file : package->files)
    {
        VR(generateMetaFileDeclare(file));
    }
    return {};
}

Result MetaGenerator::generateMetaFileDeclare(MetaFile* file)
{
    SyntaxFile* syntaxFile = (SyntaxFile*)file->syntaxObj;
    for (auto& syntaxClass : syntaxFile->classes)
    {
        MetaClass* metaClass = file->addClass(syntaxClass->name, syntaxClass);
        VR(generateMetaClassDeclare(metaClass));
    }
    return {};
}

Result MetaGenerator::generateMetaClassDeclare(MetaClass* clazz)
{
    if (clazz->step >= MetaClass::STEP_DECLARE)
    {
        return {};
    }
    clazz->step = MetaClass::STEP_DECLARE;

    SyntaxClass* syntaxClass = (SyntaxClass*)clazz->syntaxObj;

    //????????????
    for (auto& innerSyntaxClass : syntaxClass->innerClasses)
    {
        MetaClass* innerMetaClass = clazz->addInnerClass(innerSyntaxClass->name, innerSyntaxClass);
        VR(generateMetaClassDeclare(innerMetaClass));
    }

    return {};
}

Result MetaGenerator::generateMetaPackageStruct(MetaPackage* package)
{
    for (auto& sub : package->packages)
    {
        VR(generateMetaPackageStruct(sub));
    }
    for (auto& file : package->files)
    {
        VR(generateMetaFileStruct(file));
    }
    for (auto& clazz : package->classes)
    {
        VR(generateMetaClassStruct(clazz));
    }
    return {};
}
    
Result MetaGenerator::generateMetaFileStruct(MetaFile* file)
{
    SyntaxFile* syntaxFile = (SyntaxFile*)file->syntaxObj;
    for (SyntaxMulti<string>*& import : syntaxFile->imports)
    {
        MetaPackage* package = metaContainer->getPackage(import->items);
        if (package)
        {
            file->importPackages.push_back(package);
            continue;
        }

        string clazzName = import->items.back();
        import->items.pop_back();
        package = metaContainer->getPackage(import->items);
        import->items.push_back(clazzName);
        if (package)
        {
            MetaClass* clazz = package->getClass(clazzName);
            if (clazz)
            {
                file->importClasses.push_back(clazz);
                continue;
            }
        }
        
        //?????????
        return R_FAILED;
    }

    //?????????????????????
    file->importPackages.push_back(metaContainer->getPackage(KEY_ONE_STD, false));

    return {};
}
    
Result MetaGenerator::generateMetaClassStruct(MetaClass* clazz)
{
    //????????????????????????
    if (clazz->step >= MetaClass::STEP_STRUCT || clazz->isTemplateClass())
    {
        return {};
    }
    clazz->step = MetaClass::STEP_STRUCT;

    SyntaxClass* syntaxClass = (SyntaxClass*)clazz->syntaxObj;

    // ????????????
    int count = 0;
    for (auto& parent : syntaxClass->parents)
    {
        MetaType type;
        VR(generateMetaType(clazz, parent, &type)); //getOuter(),???????????????????????????
        if (type.isClass() == false)
        {
            return R_FAILED;
        }
        if (type.clazz->isInterface == false)
        {
            //????????????
            clazz->parents.push_front(type.clazz);
            count++;
        }
        else
        {
            clazz->parents.push_back(type.clazz);
        }
    }
    if (count == 0 && clazz->isInterface == false)
    {
        MetaClass* objectClass = metaContainer->getObjectClass();
        if (clazz != objectClass)
        {
            //????????????????????????object?????????
            clazz->addLinkClass(metaContainer->getObjectClass());
            clazz->parents.push_front(metaContainer->getObjectClass());
        }
    }

    // ???????????????
    for (auto& innerClass : clazz->innerClasses)
    {
        VR(generateMetaClassStruct(innerClass));
    }

    // ????????????
    for (auto& syntaxVarDef : syntaxClass->vars)
    {
        if (syntaxVarDef->type == nullptr)
        {
            //?????????????????????auto
            return R_FAILED;
        }
        MetaVariable* var = clazz->addVeriable(syntaxVarDef->name, syntaxVarDef);
        VR(generateMetaVarDefStruct(clazz, var));
    }

    // ????????????
    for (auto& syntaxFunc : syntaxClass->funcs)
    {
        if (syntaxFunc->isAnony)
        {
            //??????????????????
            continue;
        }
        MetaFunc* func = clazz->addFunction(syntaxFunc->name, syntaxFunc);
        VR(generateMetaFunctionStruct(func));
    }

    // ?????????
    VR(clazz->verifyAndRepair());

    return {};
}
    
Result MetaGenerator::generateMetaFunctionStruct(MetaFunc* func)
{
    SyntaxFunc* syntaxFunc = (SyntaxFunc*)func->syntaxObj;
    if (syntaxFunc == nullptr)
    {
        return {};
    }
    
    // ????????????
    for (auto& syntaxVarDef : syntaxFunc->params)
    {
        if (syntaxVarDef->type == nullptr)
        {
            //??????????????????auto
            return R_FAILED;
        }
        MetaVariable* param = func->addParam(syntaxVarDef->name, syntaxVarDef);
        VR(generateMetaVarDefStruct(func, param));

        //TODO:??????????????????
    }

    // ???????????????
    if (syntaxFunc->return_ == nullptr)
    {
        if (func->funcType == FUNC_CONSTRUCT)
        {
            //TODO:func->return_.setClass(func->getOuterClass()); 
        }
    }
    else
    {
        VR(generateMetaType(func->outer, syntaxFunc->return_, &func->return_));
    }

    return {};
}

Result MetaGenerator::generateMetaVarDefStruct(MetaBoxBase* box, MetaVariable* var)
{
    SyntaxVarDef* syntaxVarDef = (SyntaxVarDef*)var->syntaxObj;

    //??????
    if (syntaxVarDef->type)
    {
        VR(generateMetaType(box, syntaxVarDef->type, &var->type));
        if (var->type.isVoid())
        {
            // ????????????void??????
            return R_FAILED;
        }
    }
    else
    {
        //auto
    }

    //exp????????????????????????????????????BLOCK?????????

    return {};
}
    
Result MetaGenerator::generateMetaPackageInstruct(MetaPackage* package)
{
    for (auto& sub : package->packages)
    {
        VR(generateMetaPackageInstruct(sub));
    }
    for (auto& clazz : package->classes)
    {
        VR(generateMetaClassInstruct(clazz));
    }
    return {};
}

Result MetaGenerator::generateMetaClassInstruct(MetaClass* clazz)
{
    if (clazz->step >= MetaClass::STEP_INSTRUCT || clazz->isTemplateClass())
    {
        return {};
    }
    clazz->step = MetaClass::STEP_INSTRUCT;
    SyntaxClass* syntaxClass = (SyntaxClass*)clazz->syntaxObj;

    // ????????????
    for (auto& innerClass : clazz->innerClasses)
    {
        VR(generateMetaClassInstruct(innerClass));
    }

    // ?????????????????????
    VR(generateMetaFunctionInstruct(clazz->varInitFunc));
    VR(generateMetaFunctionInstruct(clazz->staticVarInitFunc));
    
    // ????????????
    for (auto& func : clazz->funcs)
    {
        // 
        VR(generateMetaFunctionInstruct(func));
    }

    return {};
}
    
Result MetaGenerator::generateMetaFunctionInstruct(MetaFunc* func)
{
    SyntaxFunc* syntaxFunc = (SyntaxFunc*)func->syntaxObj;
    if (syntaxFunc == nullptr || syntaxFunc->block == nullptr)
    {
        return {};
    }

    func->block = new MetaBlock(func, metaContainer, syntaxFunc->block);
    VR(generateMetaBlockInstruct(func->block));

    //????????????????????????????????????????????????????????????????????????????????????????????????
    for (auto& anonyClass : func->anonyClasses)
    {
        if (anonyClass->step != MetaClass::STEP_INSTRUCT)
        {
            return R_FAILED;
        }
    }

    return {};
}
    
Result MetaGenerator::generateMetaBlockInstruct(MetaBlock* block)
{
    SyntaxBlock* syntaxBlock = (SyntaxBlock*)block->syntaxObj;
    if (syntaxBlock == nullptr)
    {
        return {};
    }

    for (auto& element : syntaxBlock->elements)
    {
        switch (element->type)
        {
        case SyntaxElement::SENTENCE:
            {
                SyntaxExp* syntaxExp = element->sentence->exp;
                if (syntaxExp)
                {
                    MetaData tmp;
                    VR(generateMetaExpInstruct(block, syntaxExp, &tmp));
                }
            }
            break;
        case SyntaxElement::CODEBLOCK:
            {
                MetaBlock* subBlock = new MetaBlock(block, metaContainer, element->block);
                VR(generateMetaBlockInstruct(subBlock));

                MetaInstruct* instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = subBlock;
                block->instructs.push_back(instruct);
            }
            break;
        case SyntaxElement::IFBLOCK:
            {
                SyntaxIfBlock* syntaxIfBlock = element->ifBlock;

                MetaBlock* subBlock = new MetaBlock(block, metaContainer, nullptr);
                MetaInstruct* instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = subBlock;
                block->instructs.push_back(instruct);

                for (auto& ifitem : syntaxIfBlock->items)
                {
                    if (ifitem->exp)
                    {
                        MetaData cond;
                        VR(generateMetaExpInstruct(subBlock, ifitem->exp, &cond));

                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = (ifitem == syntaxIfBlock->items.front()) ? IF : ELSE_IF;
                        instruct->params.push_back(cond);
                        subBlock->instructs.push_back(instruct);
                    }
                    else
                    {
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = ELSE;
                        subBlock->instructs.push_back(instruct);
                    }

                    MetaBlock* ifItemBlock = new MetaBlock(subBlock, metaContainer, ifitem->block);
                    VR(generateMetaBlockInstruct(ifItemBlock));

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = ifItemBlock;
                    subBlock->instructs.push_back(instruct);
                }
            }
            break;
        case SyntaxElement::WHILEBLOCK:
            {
                SyntaxWhileBlock* syntaxWhileBlock = element->whileBlock;

                MetaBlock* subBlock = new MetaBlock(block, metaContainer, nullptr);
                MetaInstruct* instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = subBlock;
                block->instructs.push_back(instruct);

                if (syntaxWhileBlock->type == SyntaxWhileBlock::DOWHILE)
                {
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = DO;
                    subBlock->instructs.push_back(instruct);

                    MetaBlock* whileBlock = new MetaBlock(subBlock, metaContainer, syntaxWhileBlock->block);
                    VR(generateMetaBlockInstruct(whileBlock));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = whileBlock;
                    subBlock->instructs.push_back(instruct);

                    MetaData cond;
                    VR(generateMetaExpInstruct(subBlock, syntaxWhileBlock->exp, &cond));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = DOWHILE;
                    instruct->params.push_back(cond);
                    subBlock->instructs.push_back(instruct);
                }
                else
                {
                    MetaData cond;
                    VR(generateMetaExpInstruct(subBlock, syntaxWhileBlock->exp, &cond));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = WHILE;
                    instruct->params.push_back(cond);
                    subBlock->instructs.push_back(instruct);

                    MetaBlock* whileBlock = new MetaBlock(subBlock, metaContainer, syntaxWhileBlock->block);
                    VR(generateMetaBlockInstruct(whileBlock));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = whileBlock;
                    subBlock->instructs.push_back(instruct);
                }
            }
            break;
        case SyntaxElement::FORBLOCK:
            {
                SyntaxForBlock* syntaxForBlock = element->forBlock;

                MetaBlock* subBlock = new MetaBlock(block, metaContainer, nullptr);
                MetaInstruct* instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = subBlock;
                block->instructs.push_back(instruct);

                if (syntaxForBlock->type == SyntaxForBlock::FOR_RANGE)
                {
                    MetaVariable* var = subBlock->addVeriable(syntaxForBlock->varDef->name, syntaxForBlock->varDef);
                    VR(generateMetaVarDefStruct(block, var));

                    MetaData varData(var);
                    
                    MetaData param[2];
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[0], &param[0]));
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[1], &param[1]));
                    MetaType paramType[2];
                    paramType[0] = param[0].getType();
                    paramType[1] = param[1].getType();
                    if (paramType[0].isInteger() == false || paramType[1].isInteger() == false)
                    {
                        //???????????????
                        return R_FAILED;
                    }
                    VR(handleAutoVar(subBlock, varData, paramType[0]));
                    
                    if (metaContainer->canAutoConvertType(paramType[0], varData.getType()) == false)
                    {
                        return R_FAILED;
                    }

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = FOR_RANGE;
                    instruct->params.push_back(varData);
                    instruct->params.push_back(param[0]);
                    instruct->params.push_back(param[1]);
                    subBlock->instructs.push_back(instruct);
                }
                else if (syntaxForBlock->type == SyntaxForBlock::FOR_EACH)
                {
                    MetaVariable* var = block->addVeriable(syntaxForBlock->varDef->name, syntaxForBlock->varDef);
                    VR(generateMetaVarDefStruct(block, var));

                    MetaData varData(var);
                    
                    MetaData param;
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[0], &param));
                    MetaType paramType = param.getType();
                    if (paramType.isClass() == false)
                    {
                        return R_FAILED;
                    }
                    MetaClass* iterClass = paramType.clazz->getParentTamplateClass(metaContainer->getIterableClass());
                    if (iterClass == nullptr)
                    {
                        return R_FAILED;
                    }
                    MetaType varType = iterClass->params.front()->type;
                    VR(handleAutoVar(subBlock, varData, varType));
                    
                    if (metaContainer->canAutoConvertType(varType, varData.getType()) == false)
                    {
                        return R_FAILED;
                    }

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = FOR_EACH;
                    instruct->params.push_back(varData);
                    instruct->params.push_back(param);
                    subBlock->instructs.push_back(instruct);
                }
                else
                {
                    MetaData tmp[3];
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[0], &tmp[0]));
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[1], &tmp[1]));
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[2], &tmp[2]));

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = FOR;
                    instruct->params.push_back(tmp[0]);
                    instruct->params.push_back(tmp[1]);
                    instruct->params.push_back(tmp[2]);
                    subBlock->instructs.push_back(instruct);
                }

                MetaBlock* forBlock = new MetaBlock(subBlock, metaContainer, syntaxForBlock->block);
                VR(generateMetaBlockInstruct(forBlock));
                instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = forBlock;
                subBlock->instructs.push_back(instruct);
            }
            break;
        }
    }

    return {};
}
    
Result MetaGenerator::generateMetaExpInstruct(MetaBlock* block, SyntaxExp* syntaxExp, MetaData* out)
{
    for (auto& syntaxInstruct : syntaxExp->instructs)
    {
        VR(generateMetaInstruct(block, syntaxInstruct));
    }
    VR(generateMetaData(block, &syntaxExp->ret, out));
    return {};
}
    
Result MetaGenerator::generateMetaVarInstruct(MetaBlock* block, SyntaxVar* syntaxVar, MetaData* out)
{
    MetaFunc* inFunc = block->getOuterFunc();
    
    //.????????????????????????????????????????????????????????????
    //???????????????????????????
    //iterVarRef???iterClass?????????????????????????????????????????????
    MetaPackage* iterPackage = nullptr;
    MetaVarRef* iterVarRef = nullptr;
    MetaClass* iterClass = nullptr;

    if (syntaxVar->exp)
    {
        MetaData obj;
        VR(generateMetaExpInstruct(block, syntaxVar->exp, &obj));
        iterVarRef = new MetaVarRef(obj, metaContainer);
    }

    for (auto& item : syntaxVar->items)
    {
        bool first = syntaxVar->items.front() == item || syntaxVar->exp == nullptr;
        bool end = syntaxVar->items.back() == item;

        if (end)
        {
            //?????????????????????????????????????????????????????????????????????
            if (iterClass)
            {
                //????????????????????????
                MetaVariable* var = iterClass->getVariable(item->typeName, iterVarRef ? false : true);
                //MetaVariable* var = metaContainer->searchVariable(iterClass, item->typeName, iterVarRef ? false : true);
                if (var == nullptr)
                {
                    return R_FAILED;
                }
                iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
            }
            else if (iterVarRef)
            {
                if (iterVarRef->getType().isClass() == false)
                {
                    return R_FAILED;
                }
                //??????
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaVariable* var = clazz->getVariable(item->typeName, false);
                //MetaVariable* var = metaContainer->searchVariable(clazz, item->typeName, false);
                if (var == nullptr)
                {
                    //?????????
                    return R_FAILED;
                }
                iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
            }
            else if (iterPackage)
            {
                return R_FAILED;
            }
            else
            {
                //??????????????????
                iterVarRef = metaContainer->searchVariable(block, item->typeName, inFunc->isStatic);
                if (iterVarRef == nullptr)
                {
                    //?????????
                    return R_FAILED;
                }
            }

            out->setData(iterVarRef);
            return {};
        }
        else if (first)
        {
            //???????????????
            iterVarRef = metaContainer->searchVariable(block, item->typeName, inFunc->isStatic);
            if (iterVarRef)
            {
                continue;
            }

            //?????????
            MetaClass* clazz = metaContainer->searchClass(block, item->typeName);
            if (clazz)
            {
                MetaClass* inClass = inFunc->getOuterClass();
                if (inClass->isBaseOf(clazz))
                {
                    //???????????????
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, inClass->getThisVariable());
                }
                block->getOuterClass()->addLinkClass(clazz);
                iterClass = clazz;
                continue;
            }

            //?????????
            MetaPackage* package = metaContainer->searchPackage(block, item->typeName);
            if (package)
            {
                iterPackage = package;
                continue;
            }

            return R_FAILED;
        }
        else
        {
            if (iterPackage)
            {
                //????????????
                MetaClass* clazz = iterPackage->getClass(item->typeName);
                if (clazz)
                {
                    iterClass = clazz;
                    iterPackage = nullptr;
                    continue;
                }

                //????????????
                MetaPackage* package = iterPackage->getPackage(item->typeName);
                if (package)
                {
                    iterPackage = package;
                    continue;
                }

                return R_FAILED;
            }
            else if (iterClass)
            {
                //????????????
                MetaVariable* var = iterClass->getVariable(item->typeName, iterVarRef ? false : true);
                if (var)
                {
                    //??????????????????????????????
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    iterClass = nullptr;
                    continue;
                }

                //????????????
                MetaClass* clazz = iterClass->getInnerClass(item->typeName);
                if (clazz)
                {
                    iterVarRef = nullptr;
                    iterClass = clazz;
                    continue;
                }

                return R_FAILED;
            }
            else if (iterVarRef)
            {
                if (iterVarRef->getType().isClass() == false)
                {
                    return R_FAILED;
                }
                //??????
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaVariable* var = clazz->getVariable(item->typeName, false);
                if (var)
                {
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    continue;
                }

                //????????????
                MetaClass* innerClass = clazz->getInnerClass(item->typeName);
                if (innerClass)
                {
                    iterVarRef = nullptr;
                    iterClass = innerClass;
                    continue;
                }

                //????????????
                if (clazz->name == item->typeName)
                {
                    iterClass = clazz;
                    continue;
                }
                MetaClass* parent = clazz->getParentClass(item->typeName);
                if (parent)
                {
                    iterClass = parent;
                    continue;
                }

                return R_FAILED;
            }
            else
            {
                return R_FAILED;
            }
        }
    }

    return {};
}
    
Result MetaGenerator::generateMetaInstruct(MetaBlock* block, SyntaxInstruct* syntaxInstruct)
{
    MetaInstruct* instruct = new MetaInstruct(metaContainer, syntaxInstruct);
    instruct->cmd = syntaxInstruct->cmd;

    //????????????
    for (auto& syntaxData : syntaxInstruct->params)
    {
        MetaData data;
        VR(generateMetaData(block, &syntaxData, &data));
        instruct->params.push_back(data);
    }

    switch (instruct->cmd)
    {
    case INDEX:
        {
            //????????????
            MetaData& left = instruct->params.front();
            MetaData& right = instruct->params.back();
            MetaType type1 = left.getType();
            MetaType type2 = right.getType();
            if (metaContainer->isArray(type1) == false || type2.isInteger() == false)
            {
                return R_FAILED;
            }
            if (left.type == left.VARREF)
            {
                MetaVarRef* varRef = left.varRef;
                varRef->indexes.push_back(right);
                instruct->params.pop_back();
            }
            else
            {
                MetaVarRef* varRef = new MetaVarRef(left, metaContainer);
                varRef->indexes.push_back(right);
                instruct->params.clear();
                instruct->params.push_back(varRef);
            }
            instruct->retType = type1.clazz->params.front()->type;  //?????????????????????
            block->instructs.push_back(instruct);
        }
        break;
    case LINC:
    case LDEC:
    case RINC:
    case RDEC:
    case BITNOT:
        {
            MetaType type = instruct->params.front().getType();
            if (type.isInteger() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type;
            block->instructs.push_back(instruct);
        }
        break;
    case POS:
    case NEG:
        {
            MetaType type = instruct->params.front().getType();
            if (type.isRealNumber() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type;
            block->instructs.push_back(instruct);
        }
        break;
    case NOT:
        {
            MetaType type = instruct->params.front().getType();
            if (type.isBool() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type;
            block->instructs.push_back(instruct);
        }
        break;
    case MUL:
    case DIV:
    case REC:
    case ADD:
    case SUB:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isRealNumber() == false || type2.isRealNumber() == false)
            {
                return R_FAILED;
            }
            instruct->retType = MetaType::max(type1, type2);
            block->instructs.push_back(instruct);
        }
        break;
    case RBITMOV:
    case RBITMOV2:
    case LBITMOV:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isInteger() == false || type2.isInteger() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case GT:
    case GTE:
    case LT:
    case LTE:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isRealNumber() == false || type2.isRealNumber() == false)
            {
                return R_FAILED;
            }
            instruct->retType.setBool();
            block->instructs.push_back(instruct);
        }
        break;
    case EQ:
    case NEQ:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (((type1.isRealNumber() && type2.isRealNumber())
                || (type1.isBool() && type2.isBool())
                || ((type1.isClass() || type1.isNull()) && (type2.isClass() || type2.isNull()))) == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case EQ_DEEP:
    case NEQ_DEEP:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (((type1.isRealNumber() && type2.isRealNumber())
                || (type1.isBool() && type2.isBool())
                || ((type1.isClass() || type1.isNull()) && (type2.isClass() || type2.isNull()))) == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case BITAND:
    case BITXOR:
    case BITOR:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isInteger() == false || type2.isInteger() == false)
            {
                return R_FAILED;
            }
            instruct->retType = MetaType::max(type1, type2);
            block->instructs.push_back(instruct);
        }
        break;
    case AND:
    case OR:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isBool() == false || type2.isBool() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case ASSIGN:
    case DEEP_ASSIGN:
        {
            MetaData& left = instruct->params.front();
            MetaData& right = instruct->params.back();
            MetaType rightType = right.getType();
            VR(handleAutoVar(block, left, rightType));
            MetaType leftType = left.getType();
            VR(handleAnonyClass(block, rightType, leftType));

            if (metaContainer->canAutoConvertType(rightType, leftType) == false)
            {
                return R_FAILED;
            }
            // if (((type1.isRealNumber() && type2.isRealNumber())
            //     || (type1.isBool() && type2.isBool())
            //     || (type1.isClass() && type2.isClass())) == false)
            // {
            //     return R_FAILED;
            // }
            instruct->retType = leftType;
            block->instructs.push_back(instruct);
        }
        break;
    case MUL_ASSIGN:
    case DIV_ASSIGN:
    case REC_ASSIGN:
    case ADD_ASSIGN:
    case SUB_ASSIGN:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isRealNumber() == false || type2.isRealNumber() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case RBITMOV_ASSIGN:
    case RBITMOV2_ASSIGN:
    case LBITMOV_ASSIGN:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isInteger() == false || type2.isInteger() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case BITAND_ASSIGN:
    case BITXOR_ASSIGN:
    case BITOR_ASSIGN:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isInteger() == false || type2.isInteger() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case AND_ASSIGN:
    case OR_ASSIGN:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isBool() == false || type2.isBool() == false)
            {
                return R_FAILED;
            }
            instruct->retType = type1;
            block->instructs.push_back(instruct);
        }
        break;
    case RETURN:
        {
            MetaType type(DT_VOID);
            MetaType retType = block->getOuterFunc()->return_;
            if (instruct->params.empty() == false)
            {
                type = instruct->params.front().getType();
                VR(handleAnonyClass(block, type, retType));
            }
            if (metaContainer->canAutoConvertType(type, retType) == false)
            {
                return R_FAILED;
            }

            block->instructs.push_back(instruct);
        }
        break;
    case BREAK:
    case CONTINUE:
        {
            if (instruct->params.empty() == false)
            {
                MetaData& data = instruct->params.front();
                if (data.isConst() == false || data.getType().isInteger() == false)
                {
                    return R_FAILED;
                }
            }
            block->instructs.push_back(instruct);
        }
        break;
    case NEW:
        {
            if (syntaxInstruct->type->anony)
            {
                //?????????
                VR(generateAnonyClass(block, instruct, syntaxInstruct->type->anony, &instruct->retType));
            }
            else
            {
                VR(generateMetaType(block, syntaxInstruct->type, &instruct->retType));
                if (instruct->retType.isBaseType())
                {
                    return R_FAILED;
                }
                instruct->func = metaContainer->searchFunction(instruct->retType.clazz, instruct->retType.clazz->name, instruct->params, false);
                if (instruct->func == nullptr)
                {
                    return R_FAILED;
                }
            }
            block->instructs.push_back(instruct);
        }
        break;
    case NEW_ARRAY:
        {
            MetaType type = instruct->params.front().getType();
            if (type.isInteger() == false)
            {
                return R_FAILED;
            }
            VR(generateMetaType(block, syntaxInstruct->type, &type));
            MetaClass* newClass = nullptr;
            VR(generateRealClass(block, metaContainer->getArrayClass(), {type}, &newClass));
            instruct->retType.setClass(newClass);
            block->instructs.push_back(instruct);
        }
        break;
    case CALL:
        {
            VR(generateMetaInstructCallFunc(block, instruct));
            block->instructs.push_back(instruct);
        }
        break;
    case TYPE:
        {
            VR(generateMetaType(block, syntaxInstruct->type, &instruct->retType));
            MetaType type = instruct->params.front().getType();
            if (((type.isRealNumber() && instruct->retType.isRealNumber())
                || (type.isBool() && instruct->retType.isBool())
                || (type.isClass() && instruct->retType.isClass())) == false)
            {
                return R_FAILED;
            }
            block->instructs.push_back(instruct);
        }
        break;
    case VARDEF:
        {
            if (block->getOuterFunc()->isInitVarFunc())
            {
                MetaVariable* var = (MetaVariable*)syntaxInstruct->varDef->ptr;
                instruct->var = var;
                instruct->retType = var->type;
                block->instructs.push_back(instruct);
            }
            else
            {
                MetaVariable* var = block->addVeriable(syntaxInstruct->varDef->name, syntaxInstruct->varDef);
                VR(generateMetaVarDefStruct(block, var));
                instruct->var = var;
                instruct->retType = var->type;
                block->instructs.push_back(instruct);
            }
        }
        break;
    case BLOCK:
        {
            //??????BLOCK
        }
        break;
    default:
        break;
    }
    return {};
}

Result MetaGenerator::generateMetaInstructCallFunc(MetaBlock* block, MetaInstruct* instruct)
{
    MetaFunc* inFunc = block->getOuterFunc();
    SyntaxInstruct* syntaxInstruct = (SyntaxInstruct*)instruct->syntaxObj;
    SyntaxVar* syntaxFunc = syntaxInstruct->func;

    //.????????????????????????????????????????????????????????????
    //???????????????????????????
    //iterVarRef???iterClass?????????????????????????????????????????????
    MetaPackage* iterPackage = nullptr;
    MetaVarRef* iterVarRef = nullptr;
    MetaClass* iterClass = nullptr;

    if (syntaxFunc->exp)
    {
        MetaData obj;
        VR(generateMetaExpInstruct(block, syntaxFunc->exp, &obj));
        iterVarRef = new MetaVarRef(obj, metaContainer);
    }

    for (auto& item : syntaxFunc->items)
    {
        bool first = syntaxFunc->items.front() == item || syntaxFunc->exp == nullptr;
        bool end = syntaxFunc->items.back() == item;

        if (end)
        {
            //?????????????????????????????????????????????????????????????????????
            if (iterClass)
            {
                //????????????????????????
                MetaFunc* func = metaContainer->searchFunction(iterClass, item->typeName, instruct->params, iterVarRef ? false : true);
                if (func == nullptr)
                {
                    return R_FAILED;
                }
                if (func->isStatic == false)
                {
                    //??????this
                    instruct->params.push_front(iterVarRef);
                }
                instruct->func = func;
                instruct->cmd = CALL_FIXED;
            }
            else if (iterVarRef)
            {
                if (iterVarRef->getType().isClass() == false)
                {
                    return R_FAILED;
                }
                //??????
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaFunc* func = metaContainer->searchFunction(clazz, item->typeName, instruct->params, false);
                if (func == nullptr)
                {
                    //?????????
                    return R_FAILED;
                }
                if (func->isStatic == false)
                {
                    //??????this
                    instruct->params.push_front(iterVarRef);
                }
                instruct->func = func;
            }
            else if (iterPackage)
            {
                return R_FAILED;
            }
            else
            {
                //??????????????????
                MetaClass* clazz = block->getOuterClass();
                MetaFunc* func = metaContainer->searchFunction(clazz, item->typeName, instruct->params, inFunc->isStatic);
                if (func == nullptr)
                {
                    //?????????
                    return R_FAILED;
                }
                if (func->isStatic == false)
                {
                    //??????this
                    instruct->params.push_front(clazz->getThisVariable());
                }
                instruct->func = func;
            }
            if (instruct->func->funcType == FUNC_CONSTRUCT)
            {
                //?????????????????????????????????
                instruct->cmd = CALL_FIXED;
            }

            //??????????????????????????????
            auto iter1 = instruct->func->params.begin();
            auto iter2 = instruct->params.begin();
            for (;iter1 != instruct->func->params.end();++iter1, ++iter2)
            {
                MetaType type2 = (*iter2).getType();
                if (type2.isClass() && type2.clazz->isAnonyClass)
                {
                    //
                    handleAnonyClass(block, type2, (*iter1)->type);
                }
            }

            instruct->retType = instruct->func->return_;
            return {};
        }
        else if (first)
        {
            //???????????????
            iterVarRef = metaContainer->searchVariable(block, item->typeName, inFunc->isStatic);
            if (iterVarRef)
            {
                continue;
            }

            //?????????
            MetaClass* clazz = metaContainer->searchClass(block, item->typeName);
            if (clazz)
            {
                MetaClass* inClass = inFunc->getOuterClass();
                if (inClass->isBaseOf(clazz))
                {
                    //???????????????
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, inClass->getThisVariable());
                }
                block->getOuterClass()->addLinkClass(clazz);
                iterClass = clazz;
                continue;
            }

            //?????????
            MetaPackage* package = metaContainer->searchPackage(block, item->typeName);
            if (package)
            {
                iterPackage = package;
                continue;
            }

            return R_FAILED;
        }
        else
        {
            if (iterPackage)
            {
                //????????????
                MetaClass* clazz = iterPackage->getClass(item->typeName);
                if (clazz)
                {
                    iterClass = clazz;
                    iterPackage = nullptr;
                    continue;
                }

                //????????????
                MetaPackage* package = iterPackage->getPackage(item->typeName);
                if (package)
                {
                    iterPackage = package;
                    continue;
                }

                return R_FAILED;
            }
            else if (iterClass)
            {
                //????????????
                MetaVariable* var = iterClass->getVariable(item->typeName, iterVarRef ? false : true);
                if (var)
                {
                    //??????????????????????????????
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    iterClass = nullptr;
                    continue;
                }

                //????????????
                MetaClass* clazz = iterClass->getInnerClass(item->typeName);
                if (clazz)
                {
                    iterVarRef = nullptr;
                    iterClass = clazz;
                    continue;
                }

                return R_FAILED;
            }
            else if (iterVarRef)
            {
                if (iterVarRef->getType().isClass() == false)
                {
                    return R_FAILED;
                }
                //??????
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaVariable* var = clazz->getVariable(item->typeName, false);
                if (var)
                {
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    continue;
                }

                //????????????
                MetaClass* innerClass = clazz->getInnerClass(item->typeName);
                if (innerClass)
                {
                    iterVarRef = nullptr;
                    iterClass = innerClass;
                    continue;
                }

                //????????????
                if (clazz->name == item->typeName)
                {
                    iterClass = clazz;
                    continue;
                }
                MetaClass* parent = clazz->getParentClass(item->typeName);
                if (parent)
                {
                    iterClass = parent;
                    continue;
                }

                return R_FAILED;
            }
            else
            {
                return R_FAILED;
            }
        }
    }

    return {};
}
    
Result MetaGenerator::generateMetaType(MetaBoxBase* box, SyntaxType* syntaxType, MetaType* out)
{
    MetaClass* clazz = nullptr;
    MetaPackage* package = nullptr;
    for (auto& item : syntaxType->items)
    {
        bool first = item == syntaxType->items.front();
        bool end = item == syntaxType->items.back();

        do
        {
            if (end)
            {
                //?????????
                if (clazz)
                {
                    clazz = clazz->getInnerClass(item->typeName);
                    if (clazz == nullptr)
                    {
                        return R_FAILED;
                    }
                }
                else if (package)
                {
                    clazz = package->getClass(item->typeName);
                    if (clazz == nullptr)
                    {
                        return R_FAILED;
                    }
                }
                else
                {
                    if (item->templateTypes.empty())
                    {
                        int type = MetaType::stringToType(item->typeName);
                        if (type != DT_NONE)
                        {
                            //??????
                            out->setType(type);
                            break;
                        }

                        MetaTemplateParam* param = metaContainer->searchTemplateParam(box, item->typeName);
                        if (param)
                        {
                            *out = param->type;
                            break;
                        }
                    }
                    
                    clazz = metaContainer->searchClass(box, item->typeName);
                    if (clazz == nullptr)
                    {
                        return R_FAILED;
                    }

                    box->getOuterClass()->addLinkClass(clazz);
                }
            }
            else if (first)
            {
                clazz = metaContainer->searchClass(box, item->typeName);
                if (clazz)
                {
                    break;
                }
                box->getOuterClass()->addLinkClass(clazz);
                package = metaContainer->searchPackage(box, item->typeName);
                if (package)
                {
                    break;
                }
                return R_FAILED;
            }
            else
            {
                if (clazz)
                {
                    clazz = clazz->getInnerClass(item->typeName);
                    if (clazz)
                    {
                        break;
                    }
                }
                else if (package)
                {
                    clazz = package->getClass(item->typeName);
                    if (clazz)
                    {
                        package = nullptr;
                        break;
                    }
                    package = package->getPackage(item->typeName);
                    if (package)
                    {
                        break;
                    }
                }
                return R_FAILED;
            }
        } while (0);
        
        if (package)
        {
            if (item->templateTypes.empty() == false)
            {
                return R_FAILED;
            }
        }
        else if (clazz)
        {
            if (clazz->params.size() != item->templateTypes.size())
            {
                return R_FAILED;
            }
            if (clazz->params.empty() == false)
            {
                list<MetaType> types;
                for (auto& syntaxType2 : item->templateTypes)
                {
                    MetaType type2;
                    VR(generateMetaType(box, syntaxType2, &type2));
                    types.push_back(type2);
                }
                VR(generateRealClass(box, clazz, types, &clazz));
            }
        }
    }

    if (clazz)
    {
        out->setClass(clazz);
    }

    for (int i = 0;i < syntaxType->arrNum;i++)
    {
        MetaClass* newClass = nullptr;
        VR(generateRealClass(box, metaContainer->getArrayClass(), {*out}, &newClass));
        out->setClass(newClass);
    }

    return {};
}

Result MetaGenerator::generateMetaData(MetaBlock* block, SyntaxData* syntaxData, MetaData* out)
{
    if (syntaxData->type == SyntaxData::CONST)
    {
        return generateMetaConst(syntaxData->const_, out);
    }
    else if (syntaxData->type == SyntaxData::VAR)
    {
        return generateMetaVarInstruct(block, syntaxData->var, out);
    }
    else if (syntaxData->type == SyntaxData::VARDEF)
    {
        out->setData((MetaVariable*)syntaxData->varDef->ptr);
    }
    else if (syntaxData->type == SyntaxData::INSTRUCT)
    {
        out->setData((MetaInstruct*)syntaxData->instruct->ptr);
    }
    return {};
}

Result MetaGenerator::generateMetaConst(SyntaxConst* syntaxConst, MetaData* out)
{
    out->setData(new MetaConst(metaContainer, syntaxConst));
    out->const_->remark = syntaxConst->value;
    switch (syntaxConst->type)
    {
    case SyntaxConst::BOOLEAN:
    {
        bool bvalue = syntaxConst->value == KEY_TRUE;
        out->const_->setBoolValue(bvalue);
        break;
    }
    case SyntaxConst::NUMBER:
    {
        string number = StringUtils::toLower(syntaxConst->value);
        if ((number.find('.') != number.npos)
            || (number.find('f') != number.npos)
            || (number.find('e') != number.npos))
        {
            //??????
            char* endPtr = nullptr;
            double dvalue = ::strtod(number.c_str(), &endPtr);
            if (endPtr && *endPtr != '\0')
            {
                return R_FAILED;
            }
            if (number.back() == 'l')
            {
                out->const_->setDoubleValue(dvalue);
            }
            else if (dvalue == (float)dvalue)
            {
                //?????????????????????float
                out->const_->setFloatValue(dvalue);
            }
            else
            {
                out->const_->setDoubleValue(dvalue);
            }
        }
        else
        {
            char* endPtr = nullptr;
            long long lvalue = ::strtoll(number.c_str(), &endPtr, 0);
            if (endPtr && *endPtr != '\0')
            {
                return R_FAILED;
            }
            if (number.back() == 'l')
            {
                out->const_->setLongValue(lvalue);
            }
            else if (lvalue == (int)lvalue)
            {
                //?????????????????????int
                out->const_->setIntValue((int)lvalue);
            }
            else
            {
                out->const_->setLongValue(lvalue);
            }
        }
        break;
    }
    case SyntaxConst::STRING:
    {
        out->const_->setStringValue(syntaxConst->value);
        break;
    }
    case SyntaxConst::CHAR:
    {
        if (syntaxConst->value.empty())
        {
            out->const_->setCharValue(0);
        }
        else if (syntaxConst->value.length() == 1)
        {
            out->const_->setCharValue(syntaxConst->value[0]);
        }
        else if (syntaxConst->value.length() <= 4)
        {
            //?????????int
            int value = 0;
            for (int i = 0;i < syntaxConst->value.length();i++)
            {
                value |= ((int)(unsigned char)syntaxConst->value[i]) << (i * 8);
            }
            out->const_->setIntValue(value);
        }
        else if (syntaxConst->value.length() <= 8)
        {
            long long value = 0;
            for (int i = 0;i < syntaxConst->value.length();i++)
            {
                value |= ((long long)(unsigned char)syntaxConst->value[i]) << (i * 8);
            }
            out->const_->setLongValue(value);
        }
        else
        {
            return R_FAILED;
        }
        break;
    }
    case SyntaxConst::REFNULL:
    {
        out->const_->setRefNull();
        break;
    }
    }
    return {};
}

Result MetaGenerator::generateRealClass(MetaBoxBase* box, MetaClass* clazz, const list<MetaType>& types, MetaClass** out)
{
    MetaClass* realClass = clazz->createRealClass(types);
    if (realClass == nullptr)
    {
        return R_FAILED;
    }

    int step = box->getOuterClass()->step;
    if (step >= MetaClass::STEP_DECLARE)
    {
        VR(generateMetaClassDeclare(realClass));
    }
    if (step >= MetaClass::STEP_STRUCT)
    {
        VR(generateMetaClassStruct(realClass));
    }
    if (step >= MetaClass::STEP_INSTRUCT)
    {
        VR(generateMetaClassInstruct(realClass));
    }

    box->getOuterClass()->addLinkClass(realClass);

    *out = realClass;
    return {};
}
    
Result MetaGenerator::generateAnonyClass(MetaBoxBase* box, MetaInstruct* instruct, SyntaxClass* syntaxAnonyClass, MetaType* out)
{
    //???????????????????????????????????????????????????
    MetaFunc* outFunc = box->getOuterFunc();
    MetaClass* outClass = box->getOuterClass();
    MetaClass* clazz = outFunc->addAnonyClass(box, syntaxAnonyClass);

    //????????????
    outClass->addLinkClass(clazz);

    //????????????????????????
    MetaFunc* conFunc = clazz->addFunction(clazz->name, FUNC_CONSTRUCT);
    conFunc->block = new MetaBlock(conFunc, metaContainer, nullptr);

    //????????????????????????
    clazz->anonyConstructFunc = conFunc;
    clazz->newAnonyInstruct = instruct;

    //????????????????????????
    if (outFunc->isStatic == false)
    {
        list<MetaVariable*> vars = outClass->getAnonyThises();
        vars.push_front(outClass->getThisVariable());

        int index = 0;
        for (auto& var : vars)
        {
            clazz->addAnonyThis(++index, MetaVarRef::makeVarRef(metaContainer, nullptr, var));
            clazz->addLinkClass(var->type.clazz);
        }
    }
    instruct->func = conFunc;
    
    //??????????????????????????????????????????????????????
    if (syntaxAnonyClass->parents.empty() == false)
    {
        //?????????????????????
        VR(generateAnonyClassInstruct(box, clazz));
    }

    out->setClass(clazz);
    return {};
}
    
Result MetaGenerator::generateAnonyClassInstruct(MetaBoxBase* box, MetaClass* anony)
{
    VR(generateMetaClassDeclare(anony));
    VR(generateMetaClassStruct(anony));

    //??????????????????
    //?????????
    //1. ???????????????????????????
    //2. ????????????????????????????????????????????????
    //3. ??????????????????
    SyntaxClass* syntaxClass = (SyntaxClass*)anony->syntaxObj;
    SyntaxFunc* syntaxAnonyFunc = nullptr;
    for (auto& syntaxFunc : syntaxClass->funcs)
    {
        if (syntaxFunc->isAnony)
        {
            if (syntaxAnonyFunc)
            {
                return R_FAILED;
            }
            syntaxAnonyFunc = syntaxFunc;
        }
    }
    if (syntaxAnonyFunc)
    {
        MetaClass* interface = nullptr;
        for (auto& parent : anony->parents)
        {
            if (parent->isInterface)
            {
                interface = parent;
                break;
            }
        }
        if (interface == nullptr || interface->funcs.size() != 1)
        {
            return R_FAILED;
        }

        //
        MetaFunc* overrideFunc = interface->funcs.front();
        if (overrideFunc->params.size() != syntaxAnonyFunc->params.size())
        {
            return R_FAILED;
        }
        syntaxAnonyFunc->name = overrideFunc->name;     //
        MetaFunc* func = anony->addFunction(syntaxAnonyFunc->name, syntaxAnonyFunc);

        //??????
        auto iter1 = overrideFunc->params.begin();
        auto iter2= syntaxAnonyFunc->params.begin();
        for (;iter1 != overrideFunc->params.end();++iter1, ++iter2)
        {
            SyntaxVarDef* syntaxVarDef = *iter2;
            MetaVariable* overrideVar = *iter1;
            
            MetaVariable* param = func->addParam(syntaxVarDef->name, syntaxVarDef);
            param->type = overrideVar->type;
        }

        //?????????
        func->return_ = overrideFunc->return_;
    }

    //
    VR(generateMetaClassInstruct(anony));

    return {};
}
    
Result MetaGenerator::handleAnonyClass(MetaBoxBase* box, MetaType& anony, MetaType& dst)
{
    if (anony.isClass() && anony.clazz->isAnonyClass && anony.clazz->step == MetaClass::STEP_READY)
    {
        if (dst.isClass() == false)
        {
            return R_FAILED;
        }
        anony.clazz->parents.push_back(dst.clazz);
        return generateAnonyClassInstruct(box, anony.clazz);
    }
    return {};
}
    
Result MetaGenerator::handleAutoVar(MetaBoxBase* box, MetaData& autoVar, MetaType& dst)
{
    if (autoVar.type == MetaData::VAR && autoVar.var->type.isNone())
    {
        //?????????????????????
        autoVar.var->type = dst;
    }
    return {};
}
