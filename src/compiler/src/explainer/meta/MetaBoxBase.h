#pragma once
#include "MetaBase.h"

class MetaClass;
class MetaFunc;
class MetaBlock;
class MetaFile;
class MetaPackage;
class MetaVariable;

class MetaBoxBase : public MetaBase
{
public:
    MetaBoxBase(int boxType, MetaBoxBase* outer, MetaContainer* metaContainer, SyntaxBase* syntaxObj);

    MetaPackage* convertPackage();
    MetaClass* convertClass();
    MetaFunc* convertFunc();
    MetaBlock* convertBlock();
    MetaFile* convertFile();
    bool isPackage();
    bool isClass();
    bool isFunc();
    bool isBlock();
    bool isFile();
    MetaFile* getOuterFile();
    MetaPackage* getOuterPackage();
    MetaClass* getOuterClass();
    MetaFunc* getOuterFunc();

    virtual MetaVariable* getVariable(const string& name, bool onlyStatic);

public:
    enum
    {
        BOX_NONE,
        BOX_CLASS,
        BOX_FUNC,
        BOX_BLOCK,
        BOX_FILE,
        BOX_PACKAGE,
    };
    int boxType = BOX_NONE;
    MetaBoxBase* outer = nullptr;
};

