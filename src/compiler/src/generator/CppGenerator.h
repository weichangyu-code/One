#pragma once
#include "../common/Result.h"
#include "../explainer/meta/MetaType.h"
#include "cpp/CppContainer.h"

class MetaFile;
class MetaClass;
class MetaFunc;
class MetaVariable;
class MetaBlock;
class MetaData;
class MetaInstruct;
class MetaPackage;
class MetaContainer;
class CGBase;

/**
 * 几种类的区别
 * 原生类：只有头文件
 * 实体类：有源文件、头文件
 * 模板类：有头文件
 * 模板实体类：没文件
 * 接口类：没有源文件
 */

class CppData
{
public:
    bool reference = false;
    bool innerPoint = false;        //例如this、super
    string data;
    MetaType type;
};

class CppGenerator
{
public:
    CppGenerator(MetaContainer* metaContainer);
    ~CppGenerator();

public:
    Result generate(const string& exeName, const string& mainClass, const string& folder);

    void   addNativeClass(const string& oneClassPath, const string& hPath, const string& cppClass);
    void   addIncludeFolder(const string& include);
    void   addLibrary(const string& name);
    void   addLibraryFolder(const string& path);

protected:
    void generateCppInfoPackage(const string& path, const string& namePrefix, MetaPackage* metaPackage);
    void generateCppInfoClass(const string& path, const string& namePrefix, MetaClass* metaClass);
    void generateCppInfoFunc(const string& path, const string& namePrefix, MetaFunc* metaFunc);
    void generateCppAllClassName();
    void generateCppClassName(MetaClass* metaClass);

    Result generateMainFile(const string& root, const string& mainClass);
    void   generateMainInitStaticVar(ofstream& f, set<MetaClass*>& called, MetaClass* metaClass);
    Result generateCMakeList(const string& root, const string& exeName);

    Result generatePackage(const string& root, MetaPackage* metaPackage);
    Result generateClass(const string& root, MetaClass* metaClass);
    Result generateNativeClass(const string& root, MetaClass* metaClass);
    Result generateInterface(const string& root, MetaClass* metaClass);
    Result generateNativeInterface(const string& root, MetaClass* metaClass);
    Result generateFactoryClass(ofstream& f, MetaClass* metaClass);
    Result generateBlock(ofstream& f, const string& space, MetaBlock* block);
    Result generateInstruct(const string& space, MetaInstruct* instruct);
    Result generateInstructs(const string& space, const list<MetaInstruct*>& instructs);
    Result generateExpCode(const list<MetaInstruct*>& instructs, string& code);

    Result generateFuncDeclare(ofstream& h, MetaFunc* metaFunc);
    Result generateFuncImpl(ofstream& f, const string& space, MetaFunc* metaFunc, bool classPrefix);

    string generateType(const MetaType& type);
    string generateVarDefType(const MetaType& type);
    string generateFuncParamType(const MetaType& type);

    string generateData(MetaData& data);
    string generateTypeData(MetaData& data, const MetaType& type, bool pointer);

    void   addNativeClassImpl(const string& oneClassPath, const string& hPath, const string& cppClass);

public:
    MetaContainer* metaContainer = nullptr;
    CppContainer cppContainer;

protected:
    list<string> includeFolders;
    list<tuple<string, string, string>> natives;
    list<string> libs;
    list<string> libFolders;

//字符串常量管理
protected:
    int getStringIndex(const string& str);
protected:
    map<string, int> stringMap;
};


