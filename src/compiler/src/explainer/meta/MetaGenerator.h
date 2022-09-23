#pragma once

#include "MetaContainer.h"
#include "Common.h"
#include "MetaBase.h"
#include "../../common/Result.h"

/**
 * 元数据生成过程
 * 1. 生成所有类的声明
 * 2. 生成所有类的结构
 * 3. 生成所有类的实现
 */

class SyntaxFile;
class SyntaxType;
class SyntaxData;
class SyntaxExp;
class SyntaxConst;
class SyntaxVar;
class SyntaxInstruct;
class SyntaxClass;
class SyntaxTypePathItem;
class MetaFile;
class MetaClass;
class MetaFunc;
class MetaVariable;
class MetaBlock;
class MetaData;
class MetaInstruct;
class MetaGenerator
{
public:
    MetaGenerator(MetaContainer* metaContainer);

public:
    Result addSyntaxFile(MetaPackage* package, const string& name, SyntaxFile* syntaxFile);
    Result generate();                                                                              //所有文件添加完成了后，开始生成

protected:
    Result generateMetaType(MetaBoxBase* box, SyntaxType* syntaxType, MetaType* out);
    Result generateMetaData(MetaBlock* block, SyntaxData* syntaxData, MetaData* out);
	Result generateMetaConst(SyntaxConst* syntaxConst, MetaData* out);

    //生成类的声明，表示有这个类，具体内容不知道
    Result generateMetaPackageDeclare(MetaPackage* package);
    Result generateMetaFileDeclare(MetaFile* file);
    Result generateMetaClassDeclare(MetaClass* clazz);

    //产生结构，不生成具体代码
    Result generateMetaPackageStruct(MetaPackage* package);
    Result generateMetaFileStruct(MetaFile* file);
    Result generateMetaClassStruct(MetaClass* clazz);
    Result generateMetaFunctionStruct(MetaFunc* func);
    Result generateMetaVarDefStruct(MetaBoxBase* box, MetaVariable* var, bool needInit);

    //产生语句
    Result generateMetaPackageInstruct(MetaPackage* package);
    Result generateMetaClassInstruct(MetaClass* clazz);
    Result generateMetaVarDefInstruct(MetaVariable* var);
    Result generateMetaFunctionInstruct(MetaFunc* func);
    Result generateMetaBlockInstruct(MetaBlock* block);
    Result generateMetaVarInstruct(MetaBlock* block, SyntaxVar* syntaxVar, MetaData* out);
    Result generateMetaExpInstruct(MetaBlock* block, SyntaxExp* syntaxExp, MetaData* out);
    Result generateMetaInstruct(MetaBlock* block, SyntaxInstruct* syntaxInstruct);
	Result generateMetaInstructCallFunc(MetaBlock* block, MetaInstruct* instruct);

    //
    Result generateRealClass(MetaBoxBase* box, MetaClass* clazz, const list<MetaType>& types, MetaClass** out);
    Result generateRealClass(MetaBoxBase* box, MetaClass* clazz, SyntaxTypePathItem* item, MetaClass** out);

    //匿名类处理
    Result generateAnonyClass(MetaBoxBase* box, MetaInstruct* instruct, SyntaxClass* syntaxAnonyClass, MetaType* out);
    Result generateAnonyClassInstruct(MetaBoxBase* box, MetaClass* anony);

    //自动类型处理
    Result handleAnonyClass(MetaBoxBase* box, MetaType& anony, MetaType& dst);
    Result handleAutoVar(MetaBoxBase* box, MetaData& autoVar, MetaType& dst);

protected:
    MetaContainer* metaContainer;
};
