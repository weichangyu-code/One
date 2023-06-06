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
#include "../syntax/SyntaxTryCatch.h"
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

    //嵌套子类
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
        
        //找不到
        return R_FAILED;
    }

    //默认添加标准库
    file->importPackages.push_back(metaContainer->getPackage(KEY_ONE_STD, false));

    return {};
}
    
Result MetaGenerator::generateMetaClassStruct(MetaClass* clazz)
{
    //模板类不能结构化
    if (clazz->step >= MetaClass::STEP_STRUCT || clazz->isTemplateClass())
    {
        return {};
    }
    clazz->step = MetaClass::STEP_STRUCT;

    SyntaxClass* syntaxClass = (SyntaxClass*)clazz->syntaxObj;

    // 解析父类
    int count = 0;
    for (auto& parent : syntaxClass->parents)
    {
        MetaType type;
        VR(generateMetaType(clazz, parent, &type)); //getOuter(),不能在内部找内嵌类
        if (type.isClass() == false)
        {
            return R_FAILED;
        }
        if (type.clazz->isInterface == false)
        {
            //类放前面
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
            //没有父类的，默认object为父类
            clazz->addLinkClass(objectClass);
            clazz->parents.push_front(objectClass);
        }
    }

    // 解析内嵌类
    for (auto& innerClass : clazz->innerClasses)
    {
        VR(generateMetaClassStruct(innerClass));
    }

    // 解析变量
    for (auto& syntaxVarDef : syntaxClass->vars)
    {
        if (syntaxVarDef->type == nullptr)
        {
            //成员变量不允许auto
            return R_FAILED;
        }
        MetaVariable* var = clazz->addVeriable(syntaxVarDef->name, syntaxVarDef);
        VR(generateMetaVarDefStruct(clazz, var, var->isConst));
    }

    // 解析方法
    for (auto& syntaxFunc : syntaxClass->funcs)
    {
        if (syntaxFunc->isAnony)
        {
            //跳过匿名方法
            continue;
        }
        MetaFunc* func = clazz->addFunction(syntaxFunc->name, syntaxFunc);
        VR(generateMetaFunctionStruct(func));
    }
    
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
    
    // 解析参数
    int noDefaultParamNum = 0;
    int defaultParamNum = 0;
    for (auto& syntaxVarDef : syntaxFunc->params)
    {
        if (syntaxVarDef->type == nullptr)
        {
            //参数也不支持auto
            return R_FAILED;
        }
        MetaVariable* param = func->addParam(syntaxVarDef->name, syntaxVarDef);
        VR(generateMetaVarDefStruct(func, param, true));

        if (syntaxVarDef->exp == nullptr)
        {
            if (defaultParamNum > 0)
            {
                //默认参数后面不能有非默认参数，只有最后几个是默认参数
                return R_FAILED;
            }
            noDefaultParamNum++;
        }
        else
        {
            defaultParamNum++;
        }

        if (param->isDynamic)
        {
            if (syntaxVarDef != syntaxFunc->params.back())
            {
                //可变参数必须是最后一个
                return R_FAILED;
            }
        }
    }

    // 解析返回值
    if (syntaxFunc->returnType == nullptr)
    {
        if (func->funcType == FUNC_CONSTRUCT)
        {
            //TODO:func->returnType.setClass(func->getOuterClass()); 
        }
    }
    else
    {
        VR(generateMetaType(func->outer, syntaxFunc->returnType, &func->returnType));
    }

    return {};
}

Result MetaGenerator::generateMetaVarDefStruct(MetaBoxBase* box, MetaVariable* var, bool needInit)
{
    SyntaxVarDef* syntaxVarDef = (SyntaxVarDef*)var->syntaxObj;

    //类型
    if (syntaxVarDef->type)
    {
        VR(generateMetaType(box, syntaxVarDef->type, &var->type));
        if (var->type.isVoid())
        {
            // 不能定义void类型
            return R_FAILED;
        }
    }
    else
    {
        //auto
    }

    if (syntaxVarDef->isConst && syntaxVarDef->exp == nullptr)
    {
        //const变量右边必须有表达式
        return R_FAILED;
    }

    
    //只有常量才需要处理
    if (syntaxVarDef->exp && needInit)
    {
        //借用下varInitFunc
        var->initBlock = new MetaBlock(box->getOuterClass()->varInitFunc, metaContainer, nullptr);
    }

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

    // 解析子类
    for (auto& innerClass : clazz->innerClasses)
    {
        VR(generateMetaClassInstruct(innerClass));
    }

    // 解析变量
    for (auto& var : clazz->vars)
    {
        if (var->isConst)
        {
            VR(generateMetaVarDefInstruct(var));
        }
    }

    // 解析变量初始化
    VR(generateMetaFunctionInstruct(clazz->varInitFunc));
    VR(generateMetaFunctionInstruct(clazz->staticVarInitFunc));
    
    // 解析方法
    for (auto& func : clazz->funcs)
    {
        // 
        VR(generateMetaFunctionInstruct(func));
    }

    return {};
}

Result MetaGenerator::generateMetaVarDefInstruct(MetaVariable* var)
{
    //只有常量才需要处理
    if (var->initBlock)
    {
        SyntaxVarDef* syntaxVarDef = (SyntaxVarDef*)var->syntaxObj;

        MetaData tmp;
        VR(generateMetaExpInstruct(var->initBlock, syntaxVarDef->exp, &tmp));
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

    //处理初始化参数
    for (auto& var : func->params)
    {
        VR(generateMetaVarDefInstruct(var));
    }

    func->block = new MetaBlock(func, metaContainer, syntaxFunc->block);
    VR(generateMetaBlockInstruct(func->block));

    //所有匿名类都要实例化，不然说明是个没有赋值，无法辨别类型的匿名类
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
                block->addInstruct(instruct);
            }
            break;
        case SyntaxElement::IFBLOCK:
            {
                SyntaxIfBlock* syntaxIfBlock = element->ifBlock;

                MetaBlock* subBlock = new MetaBlock(block, metaContainer, nullptr);
                MetaInstruct* instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = subBlock;
                block->addInstruct(instruct);

                for (auto& ifitem : syntaxIfBlock->items)
                {
                    if (ifitem->exp)
                    {
                        MetaData cond;
                        VR(generateMetaExpInstruct(subBlock, ifitem->exp, &cond));

                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = (ifitem == syntaxIfBlock->items.front()) ? IF : ELSE_IF;
                        instruct->params.push_back(cond);
                        subBlock->addInstruct(instruct);
                    }
                    else
                    {
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = ELSE;
                        subBlock->addInstruct(instruct);
                    }

                    MetaBlock* ifItemBlock = new MetaBlock(subBlock, metaContainer, ifitem->block);
                    VR(generateMetaBlockInstruct(ifItemBlock));

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = ifItemBlock;
                    subBlock->addInstruct(instruct);
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
                block->addInstruct(instruct);

                if (syntaxWhileBlock->type == SyntaxWhileBlock::DOWHILE)
                {
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = DO;
                    subBlock->addInstruct(instruct);

                    MetaBlock* whileBlock = new MetaBlock(subBlock, metaContainer, syntaxWhileBlock->block);
                    VR(generateMetaBlockInstruct(whileBlock));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = whileBlock;
                    subBlock->addInstruct(instruct);

                    MetaData cond;
                    VR(generateMetaExpInstruct(subBlock, syntaxWhileBlock->exp, &cond));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = DOWHILE;
                    instruct->params.push_back(cond);
                    subBlock->addInstruct(instruct);
                }
                else
                {
                    MetaData cond;
                    VR(generateMetaExpInstruct(subBlock, syntaxWhileBlock->exp, &cond));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = WHILE;
                    instruct->params.push_back(cond);
                    subBlock->addInstruct(instruct);

                    MetaBlock* whileBlock = new MetaBlock(subBlock, metaContainer, syntaxWhileBlock->block);
                    VR(generateMetaBlockInstruct(whileBlock));
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = whileBlock;
                    subBlock->addInstruct(instruct);
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
                block->addInstruct(instruct);

                if (syntaxForBlock->type == SyntaxForBlock::FOR_RANGE)
                {
                    MetaVariable* var = subBlock->addVeriable(syntaxForBlock->varDef->name, syntaxForBlock->varDef);
                    VR(generateMetaVarDefStruct(block, var, false));

                    MetaData varData(var);
                    
                    MetaData param[2];
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[0], &param[0]));
                    VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[1], &param[1]));
                    MetaType paramType[2];
                    paramType[0] = param[0].getType();
                    paramType[1] = param[1].getType();
                    if (paramType[0].isInteger() == false || paramType[1].isInteger() == false)
                    {
                        //必须是整数
                        return R_FAILED;
                    }
                    VR(handleAutoVar(subBlock, varData, paramType[0]));
                    
                    if (metaContainer->getAutoConvertType(paramType[0], varData.getType()) == MetaContainer::ACT_CANNT)
                    {
                        return R_FAILED;
                    }

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = FOR_RANGE;
                    instruct->params.push_back(varData);
                    instruct->params.push_back(param[0]);
                    instruct->params.push_back(param[1]);
                    subBlock->addInstruct(instruct);
                }
                else if (syntaxForBlock->type == SyntaxForBlock::FOR_EACH)
                {
                    MetaVariable* var = block->addVeriable(syntaxForBlock->varDef->name, syntaxForBlock->varDef);
                    VR(generateMetaVarDefStruct(block, var, false));

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
                    
                    if (metaContainer->getAutoConvertType(varType, varData.getType()) == MetaContainer::ACT_CANNT)
                    {
                        return R_FAILED;
                    }

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = FOR_EACH;
                    instruct->params.push_back(varData);
                    instruct->params.push_back(param);
                    subBlock->addInstruct(instruct);
                }
                else
                {
                    MetaData tmp[3];
                    if (syntaxForBlock->exp[0])
                    {
                        VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[0], &tmp[0]));
                    }
                    if (syntaxForBlock->exp[1])
                    {
                        VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[1], &tmp[1]));
                    }
                    if (syntaxForBlock->exp[2])
                    {
                        VR(generateMetaExpInstruct(subBlock, syntaxForBlock->exp[2], &tmp[2]));
                    }

                    //把变量定义都提到外层
                    for (auto& instructTmp : subBlock->instructs)
                    {
                        for (auto& paramTmp : instructTmp->params)
                        {
                            if (paramTmp.type == MetaData::INSTRUCT && paramTmp.instructTmp->cmd == VARDEF)
                            {
                                paramTmp.setData(paramTmp.instructTmp->var);
                            }
                        }
                    }

                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = FOR;
                    instruct->params.push_back(tmp[0]);
                    instruct->params.push_back(tmp[1]);
                    instruct->params.push_back(tmp[2]);
                    subBlock->addInstruct(instruct);
                }

                MetaBlock* forBlock = new MetaBlock(subBlock, metaContainer, syntaxForBlock->block);
                VR(generateMetaBlockInstruct(forBlock));
                instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = forBlock;
                subBlock->addInstruct(instruct);
            }
            break;
        case SyntaxElement::TRYCATCH:
            {
                SyntaxTryCatch* tryCatch = element->tryCatch;
                
                //添加一个BLOCK，代表整个trycatch
                MetaBlock* subBlock = new MetaBlock(block, metaContainer, nullptr);
                MetaInstruct* instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = subBlock;
                block->addInstruct(instruct);

                //添加异常变量
                MetaVariable* varE = subBlock->addVeriable("__e" + metaContainer->getAnonymous() + "__", nullptr);
                varE->type.setClass(metaContainer->getExceptionClass());
                instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = VARDEF;
                instruct->var = varE;
                subBlock->addInstruct(instruct);

                //添加tryblock
                MetaBlock* tryBlock = new MetaBlock(subBlock, metaContainer, tryCatch->tryBlock);
                VR(generateMetaBlockInstruct(tryBlock));
                instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = TRY_BLOCK;
                instruct->params.push_back(varE);
                instruct->block = tryBlock;
                subBlock->addInstruct(instruct);

                //添加if
                instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = IF;
                instruct->params.push_back(varE);
                subBlock->addInstruct(instruct);
                MetaBlock* ifBlock = new MetaBlock(subBlock, metaContainer, nullptr);
                instruct = new MetaInstruct(metaContainer, nullptr);
                instruct->cmd = BLOCK;
                instruct->block = ifBlock;
                subBlock->addInstruct(instruct);

                //
                bool haveCatchAll = false;
                bool haveCatch = false;
                for (auto catchBlock : tryCatch->catchBlocks)
                {
                    if (haveCatchAll)
                    {
                        //catch all后面不能有其他catch
                        return R_FAILED;
                    }

                    if (catchBlock->varDef)
                    {
                        MetaBlock* subCatchBlock = new MetaBlock(ifBlock, metaContainer, nullptr);

                        MetaVariable* varE2 = subCatchBlock->addVeriable(catchBlock->varDef->name, catchBlock->varDef);
                        VR(generateMetaVarDefStruct(subCatchBlock, varE2, false));

                        //新增IF语句                
                        MetaInstruct* instruct2 = new MetaInstruct(metaContainer, nullptr);
                        instruct2->cmd = IS_BASE_OF;
                        instruct2->params.push_back(varE);
                        instruct2->clazz = varE2->type.clazz;
                        ifBlock->addInstruct(instruct2);

                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = haveCatch ? ELSE_IF : IF;
                        instruct->params.push_back(instruct2);
                        ifBlock->addInstruct(instruct);

                        //添加变量定义，并赋值      
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = VARDEF;
                        instruct->var = varE2;
                        instruct->retType = varE2->type;
                        subCatchBlock->addInstruct(instruct);
                        
                        instruct2 = new MetaInstruct(metaContainer, nullptr);
                        instruct2->cmd = TYPE_CONVERT;
                        instruct2->params.push_back(varE);
                        instruct2->retType = varE2->type;
                        subCatchBlock->addInstruct(instruct2);

                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = ASSIGN;
                        instruct->params.push_back(varE2);
                        instruct->params.push_back(instruct2);
                        instruct->retType = varE2->type;
                        subCatchBlock->addInstruct(instruct);

                        //添加block
                        MetaBlock* catchBlock2 = new MetaBlock(subCatchBlock, metaContainer, catchBlock->block);
                        VR(generateMetaBlockInstruct(catchBlock2));
                        
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = BLOCK;
                        instruct->block = catchBlock2;
                        subCatchBlock->addInstruct(instruct);

                        //完成
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = BLOCK;
                        instruct->block = subCatchBlock;
                        ifBlock->addInstruct(instruct);
                    }
                    else
                    {
                        if (haveCatch)
                        {
                            instruct = new MetaInstruct(metaContainer, nullptr);
                            instruct->cmd = ELSE;
                            ifBlock->addInstruct(instruct);
                        }

                        //添加block
                        MetaBlock* catchBlock2 = new MetaBlock(ifBlock, metaContainer, catchBlock->block);
                        VR(generateMetaBlockInstruct(catchBlock2));
                        
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = BLOCK;
                        instruct->block = catchBlock2;
                        ifBlock->addInstruct(instruct);
                    }
                    
                    haveCatchAll = catchBlock->varDef == nullptr;
                    haveCatch = true;
                }

                if (haveCatchAll == false)
                {
                    //重新抛出
                    if (haveCatch)
                    {
                        instruct = new MetaInstruct(metaContainer, nullptr);
                        instruct->cmd = ELSE;
                        ifBlock->addInstruct(instruct);
                    }

                    //添加block
                    MetaBlock* catchBlock2 = new MetaBlock(ifBlock, metaContainer, nullptr);
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = BLOCK;
                    instruct->block = catchBlock2;
                    ifBlock->addInstruct(instruct);

                    //添加throw
                    instruct = new MetaInstruct(metaContainer, nullptr);
                    instruct->cmd = THROW;
                    instruct->params.push_back(varE);
                    catchBlock2->addInstruct(instruct);
                }
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
    
    //.的后面可以是什么？函数、变量、子类、包名
    //指向什么都是未知数
    //iterVarRef和iterClass可能会共存，调用指定父类的函数
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
            //最后一个必须是函数，所以前一个必须是对象或者类
            if (iterClass)
            {
                //只能访问静态变量
                MetaVariable* var = iterClass->getVariable(item->typeName, iterVarRef ? MFT_ALL : MFT_ONLY_STATIC);
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
                //对象
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaVariable* var = clazz->getVariable(item->typeName, MFT_ALL);
                if (var == nullptr)
                {
                    //找不到
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
                //代表当前对象
                iterVarRef = metaContainer->searchVariable(block, item->typeName, inFunc->isStatic ? MFT_ONLY_STATIC : MFT_ALL);
                if (iterVarRef == nullptr)
                {
                    //找不到
                    return R_FAILED;
                }
            }

            out->setData(iterVarRef);
            return {};
        }
        else if (first)
        {
            //搜索变量名
            iterVarRef = metaContainer->searchVariable(block, item->typeName, inFunc->isStatic ? MFT_ONLY_STATIC : MFT_ALL);
            if (iterVarRef)
            {
                continue;
            }

            //搜索类
            MetaClass* clazz = metaContainer->searchClass(block, item->typeName);
            if (clazz)
            {
                VR(generateRealClass(block, clazz, item, &clazz));

                MetaClass* inClass = inFunc->getOuterClass();
                if (inClass->isBaseOf(clazz))
                {
                    //代表本对象
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, inClass->getThisVariable());
                }
                block->getOuterClass()->addLinkClass(clazz);
                iterClass = clazz;
                continue;
            }

            //搜索包
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
                //搜索子类
                MetaClass* clazz = iterPackage->getClass(item->typeName);
                if (clazz)
                {
                    VR(generateRealClass(block, clazz, item, &clazz));

                    iterClass = clazz;
                    iterPackage = nullptr;
                    continue;
                }

                //搜索子包
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
                //搜索变量
                MetaVariable* var = iterClass->getVariable(item->typeName, iterVarRef ? MFT_ALL : MFT_ONLY_STATIC);
                if (var)
                {
                    //只能访问静态成员变量
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    iterClass = nullptr;
                    continue;
                }

                //搜索子类
                MetaClass* clazz = iterClass->getInnerClass(item->typeName);
                if (clazz)
                {
                    VR(generateRealClass(block, clazz, item, &clazz));

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
                //对象
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaVariable* var = clazz->getVariable(item->typeName, MFT_ALL);
                if (var)
                {
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    continue;
                }

                //搜索子类
                MetaClass* innerClass = clazz->getInnerClass(item->typeName);
                if (innerClass)
                {
                    VR(generateRealClass(block, clazz, item, &innerClass));

                    iterVarRef = nullptr;
                    iterClass = innerClass;
                    continue;
                }

                //搜索父类
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

    //转换参数
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
            //索引引用
            MetaData& left = instruct->params.front();
            MetaData& right = instruct->params.back();
            MetaType type1 = left.getType();
            MetaType type2 = right.getType();

            //查找get接口
            if (type1.isClass() == false)
            {
                return R_FAILED;
            }
            
            MetaFunc* func = metaContainer->searchClassFunction(type1.clazz, KEY_ARRAY_GET_FUNC, {right}, MFT_ONLY_NORMAL);
            if (func == nullptr)
            {
                return R_FAILED;
            }
            
            instruct->cmd = CALL;
            instruct->func = func;

            instruct->retType = instruct->func->returnType;
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
        }
        break;
    case ADD:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isRealNumber() && type2.isRealNumber())
            {
                instruct->retType = MetaType::max(type1, type2);
                block->addInstruct(instruct);
                break;
            }
            if (type1.isClass() == false)
            {
                return R_FAILED;
            }
            
            MetaData firstParam = instruct->params.front();
            MetaInstruct* callInstruct = firstParam.checkFuncReturn(KEY_COMBINE_FUNC);
            if (callInstruct)
            {
                list<MetaData> params;
                params.insert(params.end(), ++callInstruct->params.begin(), callInstruct->params.end());
                params.push_back(instruct->params.back());

                MetaFunc* func = metaContainer->searchClassFunction(type1.clazz, KEY_COMBINE_FUNC, params, MFT_ONLY_NORMAL);
                if (func)
                {
                    //移除原来的指令，新增指令
                    block->removeInstruct(callInstruct);
                    instruct->params = callInstruct->params;
                    instruct->params.push_back(params.back());
                    instruct->cmd = CALL;
                    instruct->func = func;
                    instruct->retType = func->returnType;
                    block->addInstruct(instruct);
                    break;
                }
            }

            MetaFunc* func = metaContainer->searchClassFunction(type1.clazz, KEY_COMBINE_FUNC, {instruct->params.back()}, MFT_ONLY_NORMAL);
            if (func == nullptr)
            {
                return R_FAILED;
            }

            //
            instruct->cmd = CALL;
            instruct->func = func;
            //参数顺序一致，不用调整
            instruct->retType = instruct->func->returnType;
            block->addInstruct(instruct);
        }
        break;
    case MUL:
    case DIV:
    case REC:
    case SUB:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isRealNumber() == false || type2.isRealNumber() == false)
            {
                return R_FAILED;
            }
            instruct->retType = MetaType::max(type1, type2);
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
        }
        break;
    case GT:
    case GTE:
    case LT:
    case LTE:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (type1.isRealNumber() && type2.isRealNumber())
            {
                //实数可以比大小
                instruct->retType.setBool();
                block->addInstruct(instruct);
                break;
            }

            if (type1.isClass() == false)
            {
                return R_FAILED;
            }

            MetaFunc* func = metaContainer->searchClassFunction(type1.clazz, KEY_COMPARE_FUNC, {instruct->params.back()}, MFT_ONLY_NORMAL);
            if (func == nullptr)
            {
                return R_FAILED;
            }
                
            //
            int oldCmd = instruct->cmd;
            instruct->cmd = CALL;
            instruct->func = func;
            //参数顺序一致，不用调整
            instruct->retType = func->returnType;
            block->addInstruct(instruct);

            //添加和0的比较
            MetaInstruct* instructNew = new MetaInstruct(metaContainer, syntaxInstruct);
            instructNew->cmd = oldCmd;
            instructNew->params.push_back(instruct);
            instructNew->params.push_back(metaContainer->getZeroConst());
            instructNew->retType.setBool();
            block->addInstruct(instructNew);
        }
        break;
    case EQ:
    case NEQ:
        {
            //引用和数值的比较
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();
            if (((type1.isRealNumber() && type2.isRealNumber())
                || (type1.isBool() && type2.isBool())
                || ((type1.isClass() || type1.isNull()) && (type2.isClass() || type2.isNull()))) == false)
            {
                return R_FAILED;
            }
            instruct->retType.setBool();
            block->addInstruct(instruct);
        }
        break;
    case EQ_DEEP:
    case NEQ_DEEP:
        {
            MetaType type1 = instruct->params.front().getType();
            MetaType type2 = instruct->params.back().getType();

            if ((type1.isRealNumber() && type2.isRealNumber()) 
                || (type1.isBool() && type2.isBool()) 
                || (type1.isNull() && type2.isNull()))
            {
                //不需要任何处理
                instruct->cmd = (instruct->cmd == EQ_DEEP) ? EQ : NEQ;
                instruct->retType.setBool();
                block->addInstruct(instruct);
                break;
            }

            if (type1.isClass() == false)
            {
                return R_FAILED;
            }
            MetaFunc* func = metaContainer->searchClassFunction(type1.clazz, KEY_EQUAL_FUNC, {instruct->params.back()}, MFT_ONLY_NORMAL);
            if (func == nullptr)
            {
                return R_FAILED;
            }

            //
            int oldCmd = instruct->cmd;
            instruct->cmd = CALL;
            instruct->func = func;
            //参数顺序一致，不用调整
            instruct->retType = func->returnType;
            block->addInstruct(instruct);

            if (oldCmd == NEQ_DEEP)
            {
                //添加一个或指令
                MetaInstruct* instructNot = new MetaInstruct(metaContainer, syntaxInstruct);
                instructNot->cmd = NOT;
                instructNot->params.push_back(instruct);
                instructNot->retType.setBool();
                block->addInstruct(instructNot);
            }
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
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
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

            if (instruct->cmd == DEEP_ASSIGN && rightType.isClass())
            {
                //添加一个clone调用的指令
                MetaFunc* func = metaContainer->searchClassFunction(rightType.clazz, KEY_CLONE_FUNC, {}, MFT_ONLY_NORMAL);
                if (func == nullptr)
                {
                    return R_FAILED;
                }
                
                MetaInstruct* instructClone = new MetaInstruct(metaContainer, nullptr);
                instructClone->cmd = CALL;
                instructClone->func = func;
                instructClone->params.push_back(right);
                instructClone->retType = func->returnType;
                block->addInstruct(instructClone);

                //更新右操作符
                right.setData(instructClone);
                rightType = right.getType();
            }

            if (metaContainer->getAutoConvertType(rightType, leftType) == MetaContainer::ACT_CANNT)
            {
                return R_FAILED;
            }

            //判断左操作符是不是数组get，如果是转换成set
            MetaInstruct* arrInstruct = left.checkFuncReturn(KEY_ARRAY_GET_FUNC);
            if (arrInstruct)
            {
                block->removeInstruct(arrInstruct);

                MetaFunc* func = metaContainer->searchClassFunction(arrInstruct->func->getOuterClass(), KEY_ARRAY_SET_FUNC, 
                    {arrInstruct->params.back(), right}, MFT_ONLY_NORMAL);
                if (func == nullptr)
                {
                    return R_FAILED;
                }

                arrInstruct->func = func;
                arrInstruct->params.push_back(right);
                arrInstruct->retType = func->returnType;
                arrInstruct->bind(syntaxInstruct);
                block->addInstruct(arrInstruct);
                break;
            }

            instruct->cmd = ASSIGN;
            instruct->retType = leftType;
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
        }
        break;
    case COMMA:
        {
            MetaType type = instruct->params.back().getType();
            instruct->retType = type;
            block->addInstruct(instruct);
        }
        break;
    case FMT:
        {
            MetaData& left = instruct->params.front();
            MetaType leftType = left.getType();
            if (leftType.isClass() == false)
            {
                return R_FAILED;
            }

            list<MetaData> params;
            params.insert(params.end(), ++instruct->params.begin(), instruct->params.end());
            MetaFunc* func = metaContainer->searchClassFunction(leftType.clazz, KEY_FORMAT_FUNC, params, MFT_ONLY_NORMAL);
            if (func == nullptr)
            {
                return R_FAILED;
            }
            
            instruct->cmd = CALL;
            instruct->func = func;
            //参数顺序不变
            instruct->retType = func->returnType;
            block->addInstruct(instruct);
        }
        break;
    case RETURN:
        {
            MetaType type(DT_VOID);
            MetaType retType = block->getOuterFunc()->returnType;
            if (instruct->params.empty() == false)
            {
                type = instruct->params.front().getType();
                VR(handleAnonyClass(block, type, retType));
            }
            if (metaContainer->getAutoConvertType(type, retType) == MetaContainer::ACT_CANNT)
            {
                return R_FAILED;
            }

            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
        }
        break;
    case THROW:
        {
            MetaType type = instruct->params.front().getType();
            if (type.isClass() == false || type.clazz->isBaseOf(metaContainer->getExceptionClass()) == false)
            {
                //必须是异常类型
                return R_FAILED;
            }
            block->addInstruct(instruct);
        }
        break;
    case NEW:
        {
            if (syntaxInstruct->type->anony)
            {
                //匿名类
                VR(generateAnonyClass(block, instruct, syntaxInstruct->type->anony, &instruct->retType));
            }
            else
            {
                VR(generateMetaType(block, syntaxInstruct->type, &instruct->retType));
                if (instruct->retType.isBaseType())
                {
                    return R_FAILED;
                }
                instruct->func = metaContainer->searchClassFunction(instruct->retType.clazz, instruct->retType.clazz->name, instruct->params, MFT_ONLY_NORMAL);
                if (instruct->func == nullptr)
                {
                    return R_FAILED;
                }
            }
            block->addInstruct(instruct);
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
            block->addInstruct(instruct);
        }
        break;
    case CALL:
        {
            VR(generateMetaInstructCallFunc(block, instruct));
            block->addInstruct(instruct);
        }
        break;
    case TYPE_CONVERT:
        {
            VR(generateMetaType(block, syntaxInstruct->type, &instruct->retType));
            MetaType type = instruct->params.front().getType();
            if (((type.isRealNumber() && instruct->retType.isRealNumber())
                || (type.isBool() && instruct->retType.isBool())
                || (type.isClass() && instruct->retType.isClass())) == false)
            {
                return R_FAILED;
            }
            block->addInstruct(instruct);
        }
        break;
    case VARDEF:
        {
            MetaVariable* var = block->addVeriable(syntaxInstruct->varDef->name, syntaxInstruct->varDef);
            VR(generateMetaVarDefStruct(block, var, false));
            instruct->var = var;
            instruct->retType = var->type;
            block->addInstruct(instruct);
        }
        break;
    case BLOCK:
        {
            //没有BLOCK
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

    //.的后面可以是什么？函数、变量、子类、包名
    //指向什么都是未知数
    //iterVarRef和iterClass可能会共存，调用指定父类的函数
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
            //最后一个必须是函数，所以前一个必须是对象或者类
            if (iterClass)
            {
                //只能调用静态函数
                MetaFunc* func = metaContainer->searchClassFunction(iterClass, item->typeName, instruct->params, iterVarRef ? MFT_ALL : MFT_ONLY_STATIC);
                if (func == nullptr)
                {
                    return R_FAILED;
                }
                if (func->isStatic == false)
                {
                    //存入this
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
                //对象
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaFunc* func = metaContainer->searchClassFunction(clazz, item->typeName, instruct->params, MFT_ALL);
                if (func == nullptr)
                {
                    //找不到
                    return R_FAILED;
                }
                if (func->isStatic == false)
                {
                    //存入this
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
                //代表当前对象
                MetaFunc* func = metaContainer->searchFunction(block, item->typeName, instruct->params, inFunc->isStatic ? MFT_ONLY_STATIC : MFT_ALL, &iterVarRef);
                if (func == nullptr)
                {
                    //找不到
                    return R_FAILED;
                }
                if (func->isStatic == false)
                {
                    //存入this
                    if (iterVarRef)
                    {
                        instruct->params.push_front(iterVarRef);
                    }
                    else
                    {
                        instruct->params.push_front(block->getOuterClass()->getThisVariable());
                    }
                }
                instruct->func = func;
            }
            if (instruct->func->funcType == FUNC_CONSTRUCT)
            {
                //构造函数都是指定类调用
                instruct->cmd = CALL_FIXED;
            }

            //查看是否有匿名类参数
            auto iter1 = instruct->func->params.begin();
            auto iter2 = instruct->params.begin();
            if (instruct->func->isStatic == false)
            {
                ++iter2;
            }
            MetaType type1;
            MetaType type2;
            for (;iter2 != instruct->params.end();++iter2)
            {
                type2 = (*iter2).getType();
                if (iter1 != instruct->func->params.end())
                {
                    type1 = (*iter1)->type;
                    ++iter1;
                }
                if (type2.isClass() && type2.clazz->isAnonyClass)
                {
                    //
                    VR(handleAnonyClass(block, type2, type1));
                }
            }

            //是否有可变参数，需要把可变参数类型加到link里面
            if (instruct->func->isDynamicParamFunc())
            {
                MetaType type = instruct->func->getDynamicParamType();
                if (type.isClass())
                {
                    block->getOuterClass()->addLinkClass(type.clazz);
                }
            }

            instruct->retType = instruct->func->returnType;
            return {};
        }
        else if (first)
        {
            //搜索变量名
            iterVarRef = metaContainer->searchVariable(block, item->typeName, inFunc->isStatic ? MFT_ONLY_STATIC : MFT_ALL);
            if (iterVarRef)
            {
                continue;
            }

            //搜索类
            MetaClass* clazz = metaContainer->searchClass(block, item->typeName);
            if (clazz)
            {
                VR(generateRealClass(block, clazz, item, &clazz));

                MetaClass* inClass = inFunc->getOuterClass();
                if (inClass->isBaseOf(clazz))
                {
                    //代表本对象
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, inClass->getThisVariable());
                }
                block->getOuterClass()->addLinkClass(clazz);
                iterClass = clazz;
                continue;
            }

            //搜索包
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
                //搜索子类
                MetaClass* clazz = iterPackage->getClass(item->typeName);
                if (clazz)
                {
                    VR(generateRealClass(block, clazz, item, &clazz));

                    iterClass = clazz;
                    iterPackage = nullptr;
                    continue;
                }

                //搜索子包
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
                //搜索变量
                MetaVariable* var = iterClass->getVariable(item->typeName, iterVarRef ? MFT_ALL : MFT_ONLY_STATIC);
                if (var)
                {
                    //只能访问静态成员变量
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    iterClass = nullptr;
                    continue;
                }

                //搜索子类
                MetaClass* clazz = iterClass->getInnerClass(item->typeName);
                if (clazz)
                {
                    VR(generateRealClass(block, clazz, item, &clazz));

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
                //对象
                MetaClass* clazz = iterVarRef->getType().clazz;
                MetaVariable* var = clazz->getVariable(item->typeName, MFT_ALL);
                if (var)
                {
                    iterVarRef = MetaVarRef::makeVarRef(metaContainer, iterVarRef, var);
                    continue;
                }

                //搜索子类
                MetaClass* innerClass = clazz->getInnerClass(item->typeName);
                if (innerClass)
                {
                    VR(generateRealClass(block, clazz, item, &innerClass));

                    iterVarRef = nullptr;
                    iterClass = innerClass;
                    continue;
                }

                //搜索父类
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
                //结束了
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
                            //类型
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
    if (syntaxType->ellipsis)
    {
        MetaClass* newClass = nullptr;
        VR(generateRealClass(box, metaContainer->getFixedArrayClass(), {*out}, &newClass));
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
            //浮点
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
                //精度没损失，用float
                out->const_->setFloatValue((float)dvalue);
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
            else if (lvalue == (signed char)lvalue)
            {
                out->const_->setCharValue((signed char)lvalue);
            }
            else if (lvalue == (signed short)lvalue)
            {
                out->const_->setShortValue((signed short)lvalue);
            }
            else if (lvalue == (int)lvalue)
            {
                //精度没损失，用int
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
            //转换成int
            int value = 0;
            for (int i = 0;i < (int)syntaxConst->value.length();i++)
            {
                value |= ((int)(unsigned char)syntaxConst->value[i]) << (i * 8);
            }
            out->const_->setIntValue(value);
        }
        else if (syntaxConst->value.length() <= 8)
        {
            long long value = 0;
            for (int i = 0;i < (int)syntaxConst->value.length();i++)
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

    if (realClass->getTemplateNestNum() > 20)
    {
        //嵌套太多
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
    
Result MetaGenerator::generateRealClass(MetaBoxBase* box, MetaClass* clazz, SyntaxTypePathItem* item, MetaClass** out)
{
    if (clazz->params.size() != item->templateTypes.size())
    {
        return R_FAILED;
    }
    if (clazz->params.empty())
    {
        *out = clazz;
        return {};
    }
    list<MetaType> params;
    for (auto& syntaxType : item->templateTypes)
    {
        MetaType type;
        VR(generateMetaType(box, syntaxType, &type));
        params.push_back(type);
    }
    VR(generateRealClass(box, clazz, params, out));
    return {};
}
    
Result MetaGenerator::generateAnonyClass(MetaBoxBase* box, MetaInstruct* instruct, SyntaxClass* syntaxAnonyClass, MetaType* out)
{
    //类还未实例化，因为具体类型还不确定
    MetaFunc* outFunc = box->getOuterFunc();
    MetaClass* outClass = box->getOuterClass();
    MetaClass* clazz = outFunc->addAnonyClass(box, syntaxAnonyClass);

    //相互关联
    outClass->addLinkClass(clazz);

    //添加默认构造函数
    MetaFunc* conFunc = clazz->addFunction(clazz->name, FUNC_CONSTRUCT);
    conFunc->block = new MetaBlock(conFunc, metaContainer, nullptr);

    //设置匿名相关变量
    clazz->anonyConstructFunc = conFunc;
    clazz->newAnonyInstruct = instruct;

    //设置构造函数参数
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
    
    //匿名类已经知道类型的，可以直接实例化
    if (syntaxAnonyClass->parents.empty() == false)
    {
        //已经知道类型的
        VR(generateAnonyClassInstruct(box, clazz));
    }

    out->setClass(clazz);
    return {};
}
    
Result MetaGenerator::generateAnonyClassInstruct(MetaBoxBase* box, MetaClass* anony)
{
    VR(generateMetaClassDeclare(anony));
    VR(generateMetaClassStruct(anony));
    VR(anony->verifyAndRepair());

    //处理匿名方法
    //条件，
    //1. 匿名方法只会有一个
    //2. 父类必须是接口，并且只有一个函数
    //3. 参数数量一致
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

        //参数
        auto iter1 = overrideFunc->params.begin();
        auto iter2= syntaxAnonyFunc->params.begin();
        for (;iter1 != overrideFunc->params.end();++iter1, ++iter2)
        {
            SyntaxVarDef* syntaxVarDef = *iter2;
            MetaVariable* overrideVar = *iter1;
            
            MetaVariable* param = func->addParam(syntaxVarDef->name, syntaxVarDef);
            param->type = overrideVar->type;
        }

        //返回值
        func->returnType = overrideFunc->returnType;
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
        //如果类型还没有
        autoVar.var->type = dst;
    }
    else if (autoVar.type == MetaData::INSTRUCT && autoVar.instructTmp->cmd == VARDEF && autoVar.instructTmp->var->type.isNone())
    {
        autoVar.instructTmp->var->type = dst;
        autoVar.instructTmp->retType = dst;
    }
    return {};
}
