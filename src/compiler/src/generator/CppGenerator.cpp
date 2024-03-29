﻿#include "CppGenerator.h"
#include "../explainer/meta/MetaContainer.h"
#include "../explainer/meta/MetaPackage.h"
#include "../explainer/meta/MetaFile.h"
#include "../explainer/meta/MetaClass.h"
#include "../explainer/meta/MetaFunc.h"
#include "../explainer/meta/MetaVariable.h"
#include "../explainer/meta/MetaBlock.h"
#include "../explainer/meta/MetaConst.h"
#include "../explainer/meta/MetaData.h"
#include "../explainer/meta/MetaVariable.h"
#include "../explainer/meta/MetaVarRef.h"
#include "../explainer/meta/MetaInstruct.h"
#include "../explainer/meta/MetaTemplateParam.h"
#include "FileUtils.h"
#include "FolderUtils.h"
#include "StringUtils.h"
#include "../explainer/common/Keyword.h"
#include <sstream>
#include <fstream>
#include <assert.h>
#include "cpp/CppPackage.h"
#include "cpp/CppFunc.h"
#include "cpp/CppClass.h"
#include "cpp/CppFile.h"
#include "dump/Dumper.h"
using namespace OneCommon;

CppGenerator::CppGenerator(MetaContainer* metaContainer)
{
    this->metaContainer = metaContainer;
}
    
CppGenerator::~CppGenerator()
{
    
}
    
Result CppGenerator::generate(const string& exeName, const string& mainClass, const string& folder)
{
    //避免误删的情况
    FolderUtils::removeDir(FileUtils::appendFileName(folder, "one"));

    //添加原生类
    for (auto& native : natives)
    {
        addNativeClassImpl(std::get<0>(native), std::get<1>(native), std::get<2>(native));
    }

    MetaPackage* rootPackage = metaContainer->getRootPackage();

    generateCppInfoPackage("", "", rootPackage);
    generateCppAllClassName();

    VR(generatePackage(folder, rootPackage));

    VR(generateMainFile(folder, mainClass));

    VR(generateCMakeList(folder, exeName));

    VR(generateStringArray(folder));

    Dumper dumper;
    VR(dumper.dump(metaContainer, FileUtils::appendFileName(folder, "OneMeta.inl")));

    VR(generateObjectSize(folder));

    VR(generateClassPArray(folder));

    return {};
}
    
void CppGenerator::addNativeClass(const string& oneClassPath, const string& hPath, const string& cppClass)
{
    natives.push_back(tuple<string, string, string>(oneClassPath, hPath, cppClass));
}
    
void CppGenerator::addIncludeFolder(const string& include)
{
    includeFolders.push_back(include);
}
    
void CppGenerator::addLibraryFolder(const string& path)
{
    libFolders.push_back(path);
}
    
void CppGenerator::addLibrary(const string& name)
{
    libs.push_back(name);
}
    
Result CppGenerator::generateMainFile(const string& root, const string& mainClass)
{
    string fileName = FileUtils::appendFileName(root, "main.cpp");
    ofstream cpp(fileName);
    if (cpp.is_open() == false)
    {
        return R_FAILED;
    }

    MetaClass* metaClass = metaContainer->getClass(mainClass);
    if (metaClass == nullptr)
    {
        return R_FAILED;
    }
    MetaFunc* metaFunc = metaClass->getFunction("main", MFT_ONLY_STATIC);
    if (metaFunc == nullptr)
    {
        return R_FAILED;
    }
    auto cppClass = CppClass::getCppClass(metaClass);
    auto cppFunc = CppFunc::getCppFunc(metaFunc);

    cpp << "#include \"" << cppClass->cppHPath << "\"" << endl;
    cpp << "#include \"StringPool.h\"" << endl;
    for (auto& iterMetaClass : metaContainer->getClasses())
    {
        if (iterMetaClass->isTemplateClass() == false)
        {
            auto iterCppClass = CppClass::getCppClass(iterMetaClass);
            cpp << "#include \"" << iterCppClass->cppHPath << "\"" << endl;
        }
    }
    cpp << "#include \"" << "engine/Engine.h" << "\"" << endl;
    cpp << "#include \"OneMeta.inl\"" << endl;
    cpp << "#include \"StringArray.inl\"" << endl;
    cpp << "#include \"ObjectSize.inl\"" << endl;
    cpp << "#include \"ClassPArray.inl\"" << endl;
    cpp << "#include \"MetaManager.h\"" << endl;
    cpp << "#include \"SignalHandle.h\"" << endl;
    cpp << endl;

    //生产函数体
    cpp << endl;
    cpp << "int main()" << endl;
    cpp << "{" << endl;
    cpp << "    One::initSignalHandle();" << endl;
    cpp << "    One::g_stringPool.setStringArray(stringArray, sizeof(stringArray)/sizeof(char*));" << endl;
    cpp << endl;

    cpp << "    One::g_metaManager.loadMeta(oneMeta, sizeof(oneMeta));" << endl;
    cpp << "    One::g_metaManager.loadObjectSize(objectSize, sizeof(objectSize));" << endl;
    cpp << "    One::g_metaManager.initClassP(arrayClassP);" << endl;
    cpp << endl;

    //
    cpp << "    int ret = 0;" << endl;
    cpp << "    OneCoroutine::Engine engine;" << endl;
    cpp << "    engine.createCoroutine([&ret](OneCoroutine::Coroutine* co) {" << endl;

    //调用静态初始化函数
    set<MetaClass*> called;
    for (auto& iterMetaClass : metaContainer->getClasses())
    {
        generateMainInitStaticVar(cpp, called, iterMetaClass);
    }
    cpp << endl;

    if (metaFunc->returnType.isInteger())
    {
        cpp << "        ret = One::" << cppClass->cppName << "::main();" << endl;
    }
    else
    {
        cpp << "        One::" << cppClass->cppName << "::main();" << endl;
    }
    cpp << "    });" << endl;
    cpp << "    engine.run();" << endl;
    cpp << "    return ret;" << endl;

    cpp << "}" << endl;

    return {};
}
    
void CppGenerator::generateMainInitStaticVar(ofstream& f, set<MetaClass*>& called, MetaClass* metaClass)
{
    auto cppClass = CppClass::getCppClass(metaClass);
    if (metaClass->isInterface 
        || cppClass->cppNative
        || metaClass->isTemplateClass())
    {
        return;
    }
    if (called.count(metaClass) > 0)
    {
        return;
    }
    called.insert(metaClass);

    //先调用父类
    for (auto& parent : metaClass->parents)
    {
        generateMainInitStaticVar(f, called, parent);
    }

    f << "        One::" << cppClass->cppName << "::" << KEY_INIT_STATIC_VAR_FUNC << "();" << endl;
}

Result CppGenerator::generateCMakeList(const string& root, const string& exeName)
{
    ofstream f(FileUtils::appendFileName(root, "CMakeLists.txt"));
    if (f.is_open() == false)
    {
        return R_FAILED;
    }

    f << "cmake_minimum_required(VERSION 3.0.0)" << endl;
    f << "project(" << exeName << " VERSION 0.1.0)" << endl;
    
    f << "if(MSVC)" << endl;
    f << "add_definitions(/EHa /wd4819 /wd4996)" << endl;
    f << "else()" << endl;
    f << "add_definitions(-std=c++11 -ffixed-r15 -fnon-call-exceptions)" << endl;
    f << "endif()" << endl;

    //f << "include_directories(./ " << FileUtils::appendFileName(depend, "framework/src") << " " << FileUtils::appendFileName(depend, "coroutine/src") << ")" << endl;
    f << "include_directories(\"./\")" << endl;
    for (auto& include : includeFolders)
    {
        f << "include_directories(\"" << include << "\")" << endl;
    }

    for (auto& lib : libFolders)
    {
        f << "link_directories(\"" << lib << "\")" << endl;
    }

    for (auto& metaClass : metaContainer->getClasses())
    {
        auto cppClass = CppClass::getCppClass(metaClass);
        if (metaClass->isTemplateClass() == false)
        {
            f << "set(SRC_LIST ${SRC_LIST} " << cppClass->cppPath << ")" << endl;
        }
    }
    f << "set(SRC_LIST ${SRC_LIST} main.cpp)" << endl;

    f << "add_executable(" << exeName << " ${SRC_LIST})" << endl;

// if(MSVC)
// target_link_libraries(HttpServer framework.lib coroutine.lib common.lib)
// else()
// target_link_libraries(HttpServer -lframework -lcoroutine -lcommon)
// endif()

    f << "if(MSVC)" << endl;
    f << "target_link_libraries(" << exeName;
    for (auto& lib : libs)
    {
        f << " " << lib << ".lib";
    }
    f << ")" << endl;
    f << "else()" << endl;
    f << "target_link_libraries(" << exeName;
    for (auto& lib : libs)
    {
        f << " -l" << lib << "";
    }
    f << ")" << endl;
    f << "endif()" << endl;
    f << "if(WIN32)" << endl;
    f << "  target_link_libraries(" << exeName << " wsock32 ws2_32)" << endl;
    f << "else()" << endl;
    f << "  find_package(Threads)" << endl;
    f << "  target_link_libraries(" << exeName << " ${CMAKE_THREAD_LIBS_INIT})" << endl;
    f << "endif()" << endl;

    return {};
}
    
Result CppGenerator::generateStringArray(const string& root)
{
    ofstream f(FileUtils::appendFileName(root, "StringArray.inl"));
    if (f.is_open() == false)
    {
        return R_FAILED;
    }
    
    vector<string> stringArray;
    stringArray.resize(stringMap.size());
    for (auto& pair : stringMap)
    {
        stringArray[pair.second] = pair.first;
    }

    f << "const char* stringArray[] = " << endl;
    f << "{" << endl;
    for (auto& str : stringArray)
    {
        f << KEY_TAB << "\"" << str << "\"," << endl;
    }
    f << KEY_TAB << "\"\"" << endl;
    f << "};" << endl;
    f << endl;

    return {};
}
    
Result CppGenerator::generateObjectSize(const string& root)
{
    ofstream f(FileUtils::appendFileName(root, "ObjectSize.inl"));
    if (f.is_open() == false)
    {
        return R_FAILED;
    }

    f << "unsigned int objectSize[] = " << endl;
    f << "{" << endl;

    //
    for (auto& metaClass : metaContainer->getClasses())
    {
        auto cppClass = CppClass::getCppClass(metaClass);
        if (metaClass->isTemplateClass())
        {
            f << KEY_TAB << "0," << endl;
            continue;
        }
        f << KEY_TAB << "sizeof(One::" << cppClass->cppName << ")," << endl;

        auto parents = metaClass->getParentClasses();
        f << KEY_TAB << StringUtils::itoa((int)parents.size()) << "," << endl;
        for (auto& parent : parents)
        {
            auto cppParentClass = CppClass::getCppClass(parent);
            //(intptr_t)(Object*)((String*)0)
            f << KEY_TAB << "(unsigned int)(intptr_t)(One::" << cppParentClass->cppName << "*)((One::" << cppClass->cppName << "*)0)," << endl;
        }
    }
    
    f << KEY_TAB << "0" << endl << "};" << endl;
    f << endl;

    return {};
}
    
Result CppGenerator::generateClassPArray(const string& root)
{
    ofstream f(FileUtils::appendFileName(root, "ClassPArray.inl"));
    if (f.is_open() == false)
    {
        return R_FAILED;
    }

    f << "One::Class** arrayClassP[] = " << endl;
    f << "{" << endl;
    f << KEY_TAB << "nullptr," << endl;
    for (auto& metaClass : metaContainer->getClasses())
    {
        auto cppClass = CppClass::getCppClass(metaClass);
        if (metaClass->isTemplateClass())
        {
            f << KEY_TAB << "nullptr," << endl;
            continue;
        }
        f << KEY_TAB << "&One::ClassP<One::" << cppClass->cppName << ">::clazz," << endl;
    }
    f << "};" << endl;
    f << endl;

    return {};
}

void CppGenerator::generateCppInfoPackage(const string& path, const string& namePrefix, MetaPackage* metaPackage)
{
    CppPackage* cppPackage = new CppPackage(metaPackage, &cppContainer);
    cppPackage->cppFolder = path;

    for (auto& sub : metaPackage->packages)
    {
        generateCppInfoPackage(FileUtils::appendFileName(path, sub->name), namePrefix + StringUtils::toFirstUpper(sub->name), sub);
    }
    for (auto& metaClass : metaPackage->classes)
    {
        generateCppInfoClass(path, namePrefix + StringUtils::toFirstUpper(metaClass->name), metaClass);
    }
    for (auto& metaFile : metaPackage->files)
    {
        new CppFile(metaFile, &cppContainer);
    }
}
    
void CppGenerator::generateCppInfoClass(const string& path, const string& namePrefix, MetaClass* metaClass)
{
    CppClass* cppClass = CppClass::getCppClass(metaClass, &cppContainer);
    cppClass->cppName = namePrefix;
    cppClass->cppHPath = path;
    cppClass->cppPath = path;

    for (auto& inner : metaClass->innerClasses)
    {
        generateCppInfoClass(path, namePrefix + StringUtils::toFirstUpper(inner->name), inner);
    }

    generateCppInfoFunc(path, namePrefix + StringUtils::toFirstUpper(metaClass->varInitFunc->name), metaClass->varInitFunc);
    generateCppInfoFunc(path, namePrefix + StringUtils::toFirstUpper(metaClass->staticVarInitFunc->name), metaClass->staticVarInitFunc);

    for (auto& metaFunc : metaClass->funcs)
    {
        generateCppInfoFunc(path, namePrefix + StringUtils::toFirstUpper(metaFunc->name), metaFunc);
    }
}
    
void CppGenerator::generateCppInfoFunc(const string& path, const string& namePrefix, MetaFunc* metaFunc)
{
    CppFunc* cppFunc = new CppFunc(metaFunc, &cppContainer);
    if (metaFunc->funcType == FUNC_CONSTRUCT)
    {
        cppFunc->cppName = KEY_CONSTRUCT_FUNC;
    }
    else if (metaFunc->funcType == FUNC_DESTRUCT)
    {
        cppFunc->cppName = KEY_DESTRUCT_FUNC;
    }
    else
    {
        cppFunc->cppName = metaFunc->name;
    }

    for (auto& anonyClass : metaFunc->anonyClasses)
    {
        generateCppInfoClass(path, namePrefix + StringUtils::toFirstUpper(anonyClass->name), anonyClass);
    }
}
    
void CppGenerator::generateCppAllClassName()
{
    for (auto& metaClass : metaContainer->getClasses())
    {
        generateCppClassName(metaClass);
    }
}
    
void CppGenerator::generateCppClassName(MetaClass* metaClass)
{
    auto cppClass = CppClass::getCppClass(metaClass);
    if (cppClass->step >= CppClass::STEP_CPPNAME)
    {
        return;
    }
    cppClass->step = CppClass::STEP_CPPNAME;

    if (metaClass->isTemplateClass() == false)
    {
        for (auto& param : metaClass->params)
        {
            if (param->type.isClass())
            {
                generateCppClassName(param->type.clazz);
                cppClass->cppName += CppClass::getCppClass(param->type.clazz)->cppName;
            }
            else
            {
                cppClass->cppName += StringUtils::toFirstUpper(MetaType::typeToString(param->type.type));
            }
        }
    }
    cppClass->cppHPath = FileUtils::appendFileName(cppClass->cppHPath, cppClass->cppName + ".h");
    cppClass->cppPath = FileUtils::appendFileName(cppClass->cppPath, cppClass->cppName + ".cpp");
}

Result CppGenerator::generatePackage(const string& root, MetaPackage* metaPackage)
{
    FolderUtils::createDir(FileUtils::appendFileName(root, CppPackage::getCppPackage(metaPackage)->cppFolder));

    for (auto& sub : metaPackage->packages)
    {
        VR(generatePackage(root, sub));
    }

    for (auto metaClass : metaPackage->classes)
    {
        bool native = CppClass::getCppClass(metaClass)->cppNative;
        if (metaClass->isInterface)
        {
            if (native)
            {
                VR(generateNativeInterface(root, metaClass));
            }
            else
            {
                VR(generateInterface(root, metaClass));
            }
        }
        else
        {
            if (native)
            {
                VR(generateNativeClass(root, metaClass));
            }
            else
            {
                VR(generateClass(root, metaClass));
            }
        }
    }

    return {};
}

Result CppGenerator::generateNativeClass(const string& root, MetaClass* metaClass)
{
    if (metaClass->isInterface)
    {
        return {};
    }
    auto cppClass = CppClass::getCppClass(metaClass);
    if (cppClass->cppNative == false)
    {
        return {};
    }

    ofstream h(FileUtils::appendFileName(root, cppClass->cppHPath));
    if (h.is_open() == false)
    {
        return R_FAILED;
    }

    //头文件包含
    h << "#pragma once" << endl;
    h << "#include \"ObjectPool.h\"" << endl;
    h << "#include \"MetaManager.h\"" << endl;
    h << "#include \"" << cppClass->cppNativeHPath << "\"" << endl;
    h << endl;

    h << "namespace One {" << endl << endl;
    
    //声明类
    set<MetaClass*> declared;
    for (auto& linkClass : metaClass->linkClasses)
    {
        VR(generateClassDeclare(h, linkClass, declared));
    }
    h << endl;

    //类型转换
    if (metaClass->isTemplateClass())
    {
        //如果是模板类，typedef无效
        //h << "typedef " << cppClass->cppNativeName << " " << cppClass->cppName << ";" << endl;
    }
    else
    {
        h << "typedef " << cppClass->cppNativeName << cppClass->getTemplateParamImpl() << " " << cppClass->cppName << ";" << endl;
    }

    VR(generateFactoryClassH(h, metaClass));

    h << endl << "}" << endl;

    //////////////////////////////////////////////////////////////////////////////////
    //源文件
    ofstream cpp(FileUtils::appendFileName(root, cppClass->cppPath));
    if (cpp.is_open() == false)
    {
        return R_FAILED;
    }

    cpp << "#include \"" << FileUtils::getFileName(cppClass->cppHPath) << "\"" << endl;
    cpp << "#include \"" << "StringPool.h" << "\"" << endl;
    cpp << "#include \"" << "ObjectPool.h" << "\"" << endl;
    cpp << "#include \"" << "ExceptionHelper.h" << "\"" << endl;

    set<string> includes;
    for (auto& link : metaClass->linkClasses)
    {
        includes.insert(CppClass::getCppClass(link)->cppHPath);
    }
    for (auto& name : includes)
    {
        cpp << "#include \"" << name << "\"" << endl;
    }
    cpp << endl;

    cpp << "namespace One {" << endl << endl;

    VR(generateFactoryClassCpp(cpp, metaClass));

    cpp << endl << "}" << endl;

    return {};
}
    
Result CppGenerator::generateFactoryClassH(ofstream& f, MetaClass* metaClass)
{
    if (metaClass->isInterface || metaClass->isTemplateClass())
    {
        return {};
    }
    CppClass* cppClass = CppClass::getCppClass(metaClass);

    f << "class " << cppClass->getFactoryName() << endl;
    f << "{" << endl;
    f << "public:" << endl;

    for (auto& metaFunc : metaClass->funcs)
    {
        if (metaFunc->funcType != FUNC_CONSTRUCT)
        {
            continue;
        }

        //创建对应的createObject函数
        f << KEY_TAB << "static Reference<" << cppClass->cppName << "> " << KEY_CREATE_OBJECT_FUNC << "(";
        for (auto& var : metaFunc->params)
        {
            if (var != metaFunc->params.front())
            {
                f << ", ";
            }
            f << generateFuncParamType(var->type) << " " << var->name;
        }
        f << ");" << endl;
    }

    f << "};" << endl << endl;

    return {};
}
    
Result CppGenerator::generateFactoryClassCpp(ofstream& f, MetaClass* metaClass)
{
    if (metaClass->isInterface || metaClass->isTemplateClass())
    {
        return {};
    }
    CppClass* cppClass = CppClass::getCppClass(metaClass);

    for (auto& metaFunc : metaClass->funcs)
    {
        if (metaFunc->funcType != FUNC_CONSTRUCT)
        {
            continue;
        }

        //创建对应的createObject函数
        f << "Reference<" << cppClass->cppName << "> " << cppClass->getFactoryName() << "::" << KEY_CREATE_OBJECT_FUNC << "(";
        for (auto& var : metaFunc->params)
        {
            if (var != metaFunc->params.front())
            {
                f << ", ";
            }
            f << generateFuncParamType(var->type) << " " << var->name;
        }
        f << ")" << endl;
        
        f << "{" << endl;
        
        f << KEY_TAB << "auto __var__ = (" << cppClass->cppName << "*)g_objectPool.createObject(sizeof(" << cppClass->cppName << "));" << endl;
        f << KEY_TAB << "__var__->initClass(One::ClassP<" << cppClass->cppName << ">::getClass());" << endl;
        if (cppClass->cppNative)
        {
            f << KEY_TAB << "CALL_CONSTRUCT(__var__, " << cppClass->cppName;
            for (auto& var : metaFunc->params)
            {
                f << ", ";
                f << var->name;
            }
            f << ");" << endl;
        }
        else
        {
            f << KEY_TAB << "CALL_CONSTRUCT(__var__, " << cppClass->cppName << ");" << endl;
            f << KEY_TAB << "__var__->" << KEY_INIT_VAR_FUNC << "();" << endl;
            f << KEY_TAB << "__var__->" << KEY_CONSTRUCT_FUNC << "(";
            for (auto& var : metaFunc->params)
            {
                if (var != metaFunc->params.front())
                {
                    f << ", ";
                }
                f << var->name;
            }
            f << ");" << endl;
        }
        f << KEY_TAB << "return Reference<" << cppClass->cppName << ">(__var__, true, false);" << endl;

        f << "}" << endl << endl;
    }

    return {};
}

Result CppGenerator::generateInterface(const string& root, MetaClass* metaClass)
{
    if (metaClass->isInterface == false || metaClass->isTemplateClass())
    {
        return {};
    }
    CppClass* cppClass = CppClass::getCppClass(metaClass);
    if (cppClass->cppNative)
    {
        return {};
    }

    ofstream h(FileUtils::appendFileName(root, cppClass->cppHPath));
    if (h.is_open() == false)
    {
        return R_FAILED;
    }

    //
    set<string> includes;
    for (auto& parent : metaClass->parents)
    {
        includes.insert(CppClass::getCppClass(parent)->cppHPath);
    }

    //头文件包含
    h << "#pragma once" << endl;
    for (auto& name : includes)
    {
        h << "#include \"" << name << "\"" << endl;
    }
    h << endl;

    h << "namespace One {" << endl << endl;
    
    //声明类
    set<MetaClass*> declared;
    for (auto& linkClass : metaClass->linkClasses)
    {
        VR(generateClassDeclare(h, linkClass, declared));
    }
    h << endl;

    //类的定义
    h << "class " << cppClass->cppName;
    if (metaClass->parents.empty() == false)
    {
        for (auto& parent : metaClass->parents)
        {
            if (parent == metaClass->parents.front())
            {
                h << " : ";
            }
            else
            {
                h << ", ";
            }
            h << "public " << CppClass::getCppClass(parent)->cppName;
        }
    }
    h << endl;
    h << "{" << endl;
    h << "public:" << endl;

    //构造函数
    h << KEY_TAB << cppClass->cppName << "(Object* obj);" << endl;
    h << KEY_TAB << "~" << cppClass->cppName << "();" << endl;

    //方法
    for (auto& metaFunc : metaClass->funcs)
    {
        VR(generateFuncDeclare(h, metaFunc));
    }

    h << "};" << endl << endl;
    h << "}" << endl << endl;

    //////////////////////////////////////////////////////////////////////////////
    //源文件
    ofstream cpp(FileUtils::appendFileName(root, cppClass->cppPath));
    if (cpp.is_open() == false)
    {
        return R_FAILED;
    }

    cpp << "#include \"" << FileUtils::getFileName(cppClass->cppHPath) << "\"" << endl;
    cpp << "#include \"" << "StringPool.h" << "\"" << endl;
    cpp << "#include \"" << "ObjectPool.h" << "\"" << endl;
    cpp << "#include \"" << "ExceptionHelper.h" << "\"" << endl;
    includes.clear();
    for (auto& link : metaClass->linkClasses)
    {
        includes.insert(CppClass::getCppClass(link)->cppHPath);
    }
    for (auto& name : includes)
    {
        cpp << "#include \"" << name << "\"" << endl;
    }
    cpp << endl;

    cpp << "namespace One {" << endl << endl;

    //构造析构
    cpp << cppClass->cppName << "::" << cppClass->cppName << "(Object* obj)" << endl;
    if (metaClass->parents.empty() == false)
    {
        for (auto& parent : metaClass->parents)
        {
            if (parent == metaClass->parents.front())
            {
                cpp << KEY_TAB << ": ";
            }
            else
            {
                cpp << ", ";
            }
            cpp << CppClass::getCppClass(parent)->cppName << "(obj)";
        }
        cpp << endl;
    }
    cpp << "{}" << endl;
    cpp << cppClass->cppName << "::~" << cppClass->cppName << "()" << endl;
    cpp << "{}" << endl;

    cpp << endl << "}" << endl;

    return {};
}
    
Result CppGenerator::generateNativeInterface(const string& root, MetaClass* metaClass)
{
    if (metaClass->isInterface == false)
    {
        return {};
    }
    auto cppClass = CppClass::getCppClass(metaClass);
    if (cppClass->cppNative == false)
    {
        return {};
    }

    ofstream h(FileUtils::appendFileName(root, cppClass->cppHPath));
    if (h.is_open() == false)
    {
        return R_FAILED;
    }

    //头文件包含
    h << "#pragma once" << endl;
    h << "#include \"ObjectPool.h\"" << endl;
    h << "#include \"" << cppClass->cppNativeHPath << "\"" << endl;
    h << endl;

    h << "namespace One {" << endl << endl;
    
    //声明类
    set<MetaClass*> declared;
    for (auto& linkClass : metaClass->linkClasses)
    {
        VR(generateClassDeclare(h, linkClass, declared));
    }
    h << endl;

    //类型转换
    if (metaClass->isTemplateClass())
    {
        //模板类typedef无效
        //h << "typedef " << cppClass->cppNativeName << " " << cppClass->cppName << ";" << endl;
    }
    else
    {
        h << "typedef " << cppClass->cppNativeName << cppClass->getTemplateParamImpl() << " " << cppClass->cppName << ";" << endl;
    }

    h << endl << "}" << endl;
    
    //////////////////////////////////////////////////////////////////////////////////
    //源文件
    ofstream cpp(FileUtils::appendFileName(root, cppClass->cppPath));
    if (cpp.is_open() == false)
    {
        return R_FAILED;
    }

    cpp << "#include \"" << FileUtils::getFileName(cppClass->cppHPath) << "\"" << endl;
    cpp << "#include \"" << "StringPool.h" << "\"" << endl;
    cpp << "#include \"" << "ObjectPool.h" << "\"" << endl;
    cpp << "#include \"" << "ExceptionHelper.h" << "\"" << endl;

    set<string> includes;
    for (auto& link : metaClass->linkClasses)
    {
        includes.insert(CppClass::getCppClass(link)->cppHPath);
    }
    for (auto& name : includes)
    {
        cpp << "#include \"" << name << "\"" << endl;
    }
    cpp << endl;

    cpp << "namespace One {" << endl << endl;

    cpp << endl << "}" << endl;

    return {};
}

Result CppGenerator::generateClass(const string& root, MetaClass* metaClass)
{
    if (metaClass->isInterface || metaClass->isTemplateClass())
    {
        return {};
    }

    CppClass* cppClass = CppClass::getCppClass(metaClass);
    if (cppClass->cppNative)
    {
        return {};
    }
    
    ofstream h(FileUtils::appendFileName(root, cppClass->cppHPath));
    if (h.is_open() == false)
    {
        return R_FAILED;
    }

    //头文件和类声明
    set<string> includes;
    for (auto& parent : metaClass->parents)
    {
        includes.insert(CppClass::getCppClass(parent)->cppHPath);
    }

    //头文件包含
    h << "#pragma once" << endl;
    h << "#include \"MetaManager.h\"" << endl;
    for (auto& name : includes)
    {
        h << "#include \"" << name << "\"" << endl;
    }
    h << endl;

    h << "namespace One {" << endl << endl;

    //声明类
    set<MetaClass*> declared;
    for (auto& linkClass : metaClass->linkClasses)
    {
        VR(generateClassDeclare(h, linkClass, declared));
    }
    h << endl;

    //类的定义
    h << "class " << cppClass->cppName;
    for (auto& parent : metaClass->parents)
    {
        if (parent == metaClass->parents.front())
        {
            h << " : ";
        }
        else
        {
            h << ", ";
        }
        h << "public " << CppClass::getCppClass(parent)->cppName;
    }
    h << endl;
    h << "{" << endl;
    h << "public:" << endl;
    
    //构造函数
    h << KEY_TAB << cppClass->cppName << "();" << endl;
    h << KEY_TAB << "~" << cppClass->cppName << "();" << endl;

    //变量
    for (auto& var : metaClass->vars)
    {
        h << KEY_TAB;
        if (var->isStatic)
        {
            h << "static ";
        }

        h << generateVarDefType(var->type) << " " << var->name << ";" << endl;
    }
    h << endl;

    //加上固定的方法
    VR(generateFuncDeclare(h, metaClass->varInitFunc));
    VR(generateFuncDeclare(h, metaClass->staticVarInitFunc));

    //方法
    for (auto& metaFunc : metaClass->funcs)
    {
        VR(generateFuncDeclare(h, metaFunc));
    }

    h << "};" << endl << endl;

    VR(generateFactoryClassH(h, metaClass));

    h << endl << "}" << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////
    //源文件
    ofstream cpp(FileUtils::appendFileName(root, cppClass->cppPath));
    if (cpp.is_open() == false)
    {
        return R_FAILED;
    }

    cpp << "#include \"" << FileUtils::getFileName(cppClass->cppHPath) << "\"" << endl;
    cpp << "#include \"" << "StringPool.h" << "\"" << endl;
    cpp << "#include \"" << "ObjectPool.h" << "\"" << endl;
    cpp << "#include \"" << "ExceptionHelper.h" << "\"" << endl;
    includes.clear();
    for (auto& link : metaClass->linkClasses)
    {
        includes.insert(CppClass::getCppClass(link)->cppHPath);
    }
    for (auto& name : includes)
    {
        cpp << "#include \"" << name << "\"" << endl;
    }
    cpp << endl;

    cpp << "namespace One {" << endl << endl;

    for (auto& var : metaClass->vars)
    {
        if (var->isStatic)
        {
            cpp << generateVarDefType(var->type) << " "  << cppClass->cppName << "::" << var->name << ";" << endl;
        }
    }
    cpp << endl;

    //构造析构
    cpp << cppClass->cppName << "::" << cppClass->cppName << "()" << endl;
    if (metaClass->parents.empty() == false)
    {
        int index = 0;
        for (auto& parent : metaClass->parents)
        {
            if (parent->isInterface == false)
            {
                continue;
            }
            if (index == 0)
            {
                cpp << KEY_TAB << ": ";
            }
            else
            {
                cpp << ", ";
            }
            cpp << CppClass::getCppClass(parent)->cppName << "(this)";

            index++;
        }
        if (index > 0)
        {
            cpp << endl;
        }
    }
    cpp << "{}" << endl;
    cpp << cppClass->cppName << "::~" << cppClass->cppName << "()" << endl;
    cpp << "{}" << endl;

    VR(generateFuncImpl(cpp, "", metaClass->varInitFunc, true));
    VR(generateFuncImpl(cpp, "", metaClass->staticVarInitFunc, true));

    for (auto& metaFunc : metaClass->funcs)
    {
        VR(generateFuncImpl(cpp, "", metaFunc, true));
    }
    
    VR(generateFactoryClassCpp(cpp, metaClass));

    cpp << endl << "}" << endl;

    //增加内嵌类
    for (auto& inner : metaClass->innerClasses)
    {
        if (inner->isInterface)
        {
            VR(generateInterface(root, inner));
        }
        else
        {
            VR(generateClass(root, inner));
        }
    }
    
    //添加匿名类
    for (auto& metaFunc : metaClass->funcs)
    {
        for (auto& anony : metaFunc->anonyClasses)
        {
            VR(generateClass(root, anony));
        }
    }

    return {};
}

Result CppGenerator::generateInstruct(const string& space, MetaInstruct* instruct)
{
    switch (instruct->cmd)
    {
    case NOP:
        {
            //原样处理
            instruct->cppCode = generateData(instruct->params.front());
        }
        break;
    case INDEX:
        {
            //不会进入
            //instruct->cppCode = generateData(instruct->params.front());
            assert(false);
        }
        break;
    case LINC:
        {
            instruct->cppCode = "++" + generateData(instruct->params.front());
        }
        break;
    case LDEC:
        {
            instruct->cppCode = "--" + generateData(instruct->params.front());
        }
        break;
    case RINC:
        {
            instruct->cppCode = generateData(instruct->params.front()) + "++";
        }
        break;
    case RDEC:
        {
            instruct->cppCode = generateData(instruct->params.front()) + "--";
        }
        break;
    case POS:
        {
            instruct->cppCode = "+" + generateData(instruct->params.front());
        }
        break;
    case NEG:
        {
            instruct->cppCode = "-" + generateData(instruct->params.front());
        }
        break;
    case NOT:
        {
            instruct->cppCode = "!" + generateData(instruct->params.front());
        }
        break;
    case BITNOT:
        {
            instruct->cppCode = "~" + generateData(instruct->params.front());
        }
        break;
    case MUL:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " * " + generateData(instruct->params.back()) + ")";
        }
        break;
    case DIV:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " / " + generateData(instruct->params.back()) + ")";
        }
        break;
    case REC:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " % " + generateData(instruct->params.back()) + ")";
        }
        break;
    case ADD:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " + " + generateData(instruct->params.back()) + ")";
        }
        break;
    case SUB:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " - " + generateData(instruct->params.back()) + ")";
        }
        break;
    case RBITMOV:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " >> " + generateData(instruct->params.back()) + ")";
        }
        break;
    case RBITMOV2:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " >>> " + generateData(instruct->params.back()) + ")";
        }
        break;
    case LBITMOV:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " << " + generateData(instruct->params.back()) + ")";
        }
        break;
    case GT:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " > " + generateData(instruct->params.back()) + ")";
        }
        break;
    case GTE:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " >= " + generateData(instruct->params.back()) + ")";
        }
        break;
    case LT:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " < " + generateData(instruct->params.back()) + ")";
        }
        break;
    case LTE:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " <= " + generateData(instruct->params.back()) + ")";
        }
        break;
    case EQ:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " == " + generateData(instruct->params.back()) + ")";
        }
        break;
    case NEQ:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " != " + generateData(instruct->params.back()) + ")";
        }
        break;
    case BITAND:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " & " + generateData(instruct->params.back()) + ")";
        }
        break;
    case BITXOR:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " ^ " + generateData(instruct->params.back()) + ")";
        }
        break;
    case BITOR:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " | " + generateData(instruct->params.back()) + ")";
        }
        break;
    case AND:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " && " + generateData(instruct->params.back()) + ")";
        }
        break;
    case OR:
        {
            instruct->cppCode = "(" + generateData(instruct->params.front()) + " || " + generateData(instruct->params.back()) + ")";
        }
        break;
    case COND:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateTypeData((*(++instruct->params.begin())), instruct->retType, false);
            string param2 = generateTypeData(instruct->params.back(), instruct->retType, false);
            instruct->cppCode = "(" + param0 + " ? " + param1 + " : " + param2 + ")";
        }
        break;
    case ASSIGN:
        {
            instruct->cppCode = generateData(instruct->params.front()) + " = " 
                + generateTypeData(instruct->params.back(), instruct->params.front().getType(), false);
        }
        break;
    case MUL_ASSIGN:
        {
            instruct->cppCode = generateData(instruct->params.front()) + " *= " + generateData(instruct->params.back());
        }
        break;
    case DIV_ASSIGN:
        {
            instruct->cppCode = generateData(instruct->params.front()) + " /= " + generateData(instruct->params.back());
        }
        break;
    case REC_ASSIGN:
        {
            instruct->cppCode = generateData(instruct->params.front()) + " %= " + generateData(instruct->params.back());
        }
        break;
    case ADD_ASSIGN:
        {
            instruct->cppCode = generateData(instruct->params.front()) + " += " + generateData(instruct->params.back());
        }
        break;
    case SUB_ASSIGN:
        {
            instruct->cppCode = generateData(instruct->params.front()) + " -= " + generateData(instruct->params.back());
        }
        break;
    case RBITMOV_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " >>= " + param1;            
        }
        break;
    case RBITMOV2_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " >>>= " + param1;
        }
        break;
    case LBITMOV_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " <<= " + param1;
        }
        break;
    case BITAND_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " &= " + param1;
        }
        break;
    case BITXOR_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " ^= " + param1;
        }
        break;
    case BITOR_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " |= " + param1;
        }
        break;
    case AND_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " = " + param0 + " && " + param1;
        }
        break;
    case OR_ASSIGN:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData(instruct->params.back());
            instruct->cppCode = param0 + " = " + param0 + " || " + param1;
        }
        break;
    case COMMA:
        {
            ostringstream stream;
            stream << "(";
            for (auto& param : instruct->params)
            {
                string paramString = generateData(param);
                if (&param != &instruct->params.front())
                {
                    stream << ", ";
                }
                stream << paramString;
            }
            stream << ")";
            instruct->cppCode = stream.str();
        }
        break;
    case RETURN:
        {
            if (instruct->params.empty())
            {
                instruct->cppCode = "return";
            }
            else
            {
                instruct->cppCode = "return " + generateTypeData(instruct->params.front(), instruct->retType, false);
            }
        }
        break;
    case BREAK:
        {
            instruct->cppCode = "break";
        }
        break;
    case CONTINUE:
        {
            instruct->cppCode = "continue";
        }
        break;
    case THROW:
        {
            instruct->cppCode = "throwException(" + generateData(instruct->params.front()) + ")";
        }
        break;
    case NEW:
        {
            ostringstream stream;

            CppClass* cppClass = CppClass::getCppClass(instruct->retType.clazz);
            
            stream << cppClass->getFactoryName() << "::" << KEY_CREATE_OBJECT_FUNC << "(";
            int index = 0;
            for (auto& param : instruct->params)
            {
                if (index == 0)
                {
                    stream << generateData(param);
                }
                else
                {
                    stream << ", " << generateData(param);
                }
                index++;
            }
            stream << ")";
            instruct->cppCode = stream.str();
        }
        break;
    case NEW_ARRAY:
        {
            instruct->cppCode = "Array<" + generateType(instruct->retType.clazz->params.front()->type) + ">::createArray("
                    + generateData(instruct->params.front()) +  ")";
        }
        break;
    case DELETE:
        {
            instruct->cppCode = generateData(instruct->params.front()) + "->" KEY_DESTROY_FUNC "()";
        }
        break;
    case CALL:
    case CALL_FIXED:
        {
            MetaFunc* metaFunc = instruct->func;
            auto cppFunc = CppFunc::getCppFunc(metaFunc);
            auto cppClass = CppClass::getCppClass(metaFunc->getOuterClass());

            string dynamicParam;
            if (metaFunc->isDynamicParamFunc())
            {
                MetaType type = metaFunc->getDynamicParamType();

                //将最后几个可变参数转换成数组
                ostringstream stream;
                stream << "Array<" << generateType(type) << ">::createArray({";

                int staticParamNum = (metaFunc->isStatic ? 0 : 1) + ((int)metaFunc->params.size() - 1);
                int i = 0;
                for (auto iter = instruct->params.begin();iter != instruct->params.end();)
                {
                    if (i >= staticParamNum)
                    {
                        if (i > staticParamNum)
                        {
                            stream << ", ";
                        }
                        stream << generateTypeData(*iter, type, false);

                        instruct->params.erase(iter++);
                    }
                    else
                    {
                        ++iter;
                    }
                    i++;
                }

                stream << "})";
                dynamicParam = stream.str();
            }

            ostringstream stream;
            if (metaFunc->isStatic)
            {
                //直接调用
                stream << cppClass->cppName << "::" << cppFunc->cppName << "(";
                int index = 0;
                auto paramTypeIter = metaFunc->params.begin();
                for (auto& param : instruct->params)
                {
                    if (index == 0)
                    {
                        //stream << generateData(param);
                        stream << generateTypeData(param, (*paramTypeIter)->type, true);
                    }
                    else
                    {
                        //stream << ", " << generateData(param);
                        stream << ", " << generateTypeData(param, (*paramTypeIter)->type, true);
                    }
                    ++paramTypeIter;
                    index++;
                }
            }
            else
            {
                int index = 0;
                auto paramTypeIter = metaFunc->params.begin();
                for (auto& param : instruct->params)
                {
                    if (index == 0)
                    {
                        if (instruct->cmd == CALL_FIXED)
                        {
                            //调用类函数，不会触发虚函数
                            stream << generateData(param) << "->" << cppClass->cppName << "::" << cppFunc->cppName << "(";
                        }
                        else
                        {
                            stream << generateData(param) << "->" << cppFunc->cppName << "(";
                        }
                    }
                    else if (index == 1)
                    {
                        //stream << generateData(param);
                        stream << generateTypeData(param, (*paramTypeIter)->type, true);
                        ++paramTypeIter;
                    }
                    else
                    {
                        //stream << ", " << generateData(param);
                        stream << ", " << generateTypeData(param, (*paramTypeIter)->type, true);
                        ++paramTypeIter;
                    }

                    index++;
                }
            }

            if (metaFunc->isDynamicParamFunc())
            {
                if (metaFunc->params.size() > 1)
                {
                    stream << ", ";
                }
                stream << dynamicParam;
            }

            stream << ")";
            instruct->cppCode = stream.str();
        }
        break;
    case TYPE_CONVERT:
        {
            MetaType& typeDst = instruct->retType;
            MetaType typeSrc = instruct->params.front().getType();
            if (metaContainer->getAutoConvertType(typeSrc, typeDst) != MetaContainer::ACT_CANNT)
            {
                //可以自动转换
                instruct->cppCode = generateTypeData(instruct->params.front(), typeDst, false);
            }
            else
            {
                if (typeSrc.isRealNumber() && typeDst.isRealNumber())
                {
                    instruct->cppCode = "(" + generateType(typeDst) + ")" + generateData(instruct->params.front());
                }
                else if (typeSrc.isClass() && typeDst.isClass())
                {
                    CppClass* cppClass = CppClass::getCppClass(typeDst.clazz);
                    if (typeSrc.clazz->isInterface)
                    {
                        //g_metaManager.convertInterfaceType<One::XXX>((const Reference<Interface>&)generateData(instruct->params.front()))
                        instruct->cppCode = "g_metaManager.convertInterfaceType<One::" + cppClass->cppName + ">"
                                                "((const Reference<Interface>&)(" + generateData(instruct->params.front()) + "))";
                    }
                    else
                    {
                        instruct->cppCode = "g_metaManager.convertObjectType<One::" + cppClass->cppName + ">"
                                                "((const Reference<Object>&)(" + generateData(instruct->params.front()) + "))";
                    }
                }
                else
                {
                    return R_FAILED;
                }
            }
        }
        break;
    case VARDEF:
        {
            MetaVariable* var = instruct->var;
            string prefix;
            if (var->isConst)
            {
                prefix += "const ";
            }
            if (var->isStatic)
            {
                prefix += "static ";
            }
            if (var->isReference)
            {
                //引用
                instruct->cppCode = prefix + generateVarDefType(var->type) + "& " + var->name;
            }
            else
            {
                instruct->cppCode = prefix + generateVarDefType(var->type) + " " + var->name;
            }
        }
        break;
    case IF:
        {
            instruct->cppCode = "if (" + generateData(instruct->params.front()) + ")";
        }
        break;
    case ELSE_IF:
        {
            instruct->cppCode = "else if (" + generateData(instruct->params.front()) + ")";
        }
        break;
    case ELSE:
        {
            instruct->cppCode = "else";
        }
        break;
    case WHILE:
        {
            instruct->cppCode = "while (" + generateData(instruct->params.front()) + ")";
        }
        break;
    case DO:
        {
            instruct->cppCode = "do";
        }
        break;
    case DOWHILE:
        {
            instruct->cppCode = "while (" + generateData(instruct->params.front()) + ")";
        }
        break;
    case FOR:
        {
            string param0 = generateData(instruct->params.front());
            string param1 = generateData((*(++instruct->params.begin())));
            string param2 = generateData(instruct->params.back());
            instruct->cppCode = "for (" + param0 + ";" + param1 + ";" + param2 + ")";
        }
        break;
    case FOR_RANGE:
        {
            MetaVariable* var = instruct->params.front().var;
            string param0 = generateData((*(++instruct->params.begin())));
            string param1 = generateData(instruct->params.back());

            ostringstream ss;
            ss << space << "auto __begin__ = " << param0 << ";" << endl;
            ss << space << "auto __end__ = " << param1 << ";" << endl;
            ss << space << "for (" << "auto " << var->name << " = __begin__;" << var->name << " < __end__;" + var->name + "++)" << endl;
            instruct->cppCode = ss.str();
        }
        break;
    case FOR_EACH:
        {
            MetaVariable* var = instruct->params.front().var;
            string param0 = generateData((*(++instruct->params.begin())));

            ostringstream ss;
            ss << space << "auto __iter__ = " << param0 << "->iterator();" << endl;
            ss << space << "auto __hasNext__ = __iter__->hasNext();" << endl;
            ss << space << "if (__hasNext__)" << endl;
            ss << space << "for (auto " << var->name << "=__iter__->next();__hasNext__;__hasNext__=__iter__->hasNext(),__hasNext__?(" << var->name << "=__iter__->next(),true):false)" << endl;
            instruct->cppCode = ss.str();
        }
        break;
    case IS_BASE_OF:
        {
            CppClass* cppClass = CppClass::getCppClass(instruct->clazz);
            string param0 = generateData(instruct->params.front());
            instruct->cppCode = param0 + "->getClass()->isBaseOf(ClassP<" + cppClass->cppName + ">::getClass())";
        }
        break;
    }
    return {};
}
    
Result CppGenerator::generateInstructs(const string& space, const list<MetaInstruct*>& instructs)
{
    for (auto& instruct : instructs)
    {
        VR(generateInstruct(space, instruct));
    }
    return {};
}
    
Result CppGenerator::generateExpCode(const list<MetaInstruct*>& instructs, string& code)
{
    VR(generateInstructs("", instructs));

    if (instructs.empty() == false)
    {
        code = std::move(instructs.back()->cppCode);
    }

    return {};
}
    
Result CppGenerator::generateClassDeclare(ofstream& h, MetaClass* metaClass, set<MetaClass*>& declared)
{
    if (declared.count(metaClass) > 0)
    {
        return {};
    }
    declared.insert(metaClass);

    CppClass* cppClass = CppClass::getCppClass(metaClass);
    if (cppClass->cppNative)
    {
        //遍历模板参数
        for (auto& param : metaClass->params)
        {
            if (param->type.isClass())
            {
                VR(generateClassDeclare(h, param->type.clazz, declared));
            }
        }

        //定义模板
        if (metaClass->params.empty())
        {
            h << "class " << cppClass->cppNativeName << ";" << endl;
        }
        else
        {
            h << cppClass->getTemplateParamDefine() << " class " << cppClass->cppNativeName << ";" << endl;
        }

        //定义类
        h << "typedef " << cppClass->cppNativeName << cppClass->getTemplateParamImpl() << " " << cppClass->cppName << ";" << endl;
    }
    else
    {
        h << "class " << cppClass->cppName << ";" << endl;
    }

    return {};
}

Result CppGenerator::generateBlock(ofstream& f, const string& space, MetaBlock* block)
{
    VR(generateInstructs(space, block->instructs));

    for (auto& instruct : block->instructs)
    {
        if (instruct->cmd == BLOCK)
        {
            f << space << "{" << endl;
            VR(generateBlock(f, space + KEY_TAB, instruct->block));
            f << space << "}" << endl;
        }
        else if (instruct->cmd == TRY_BLOCK)
        {
            f << space << "try {" << endl;
            VR(generateBlock(f, space + KEY_TAB, instruct->block));
            f << space << "} catch (ExceptionBox box) {" << endl;
            f << space + KEY_TAB << instruct->params.front().var->name << " = box.e;" << endl;
            f << space << "}" << endl;
        }
        else if (instruct->cmd == IF || instruct->cmd == ELSE_IF || instruct->cmd == ELSE
            || instruct->cmd == DO || instruct->cmd == WHILE || instruct->cmd == FOR)
        {
            f << space << instruct->cppCode << endl;
        }
        else if (instruct->cmd == FOR_RANGE || instruct->cmd == FOR_EACH)
        {
            f << instruct->cppCode;
        }
        else if (instruct->cppCode.empty() == false)
        {
            f << space << instruct->cppCode << ";" << endl;
        }
    }

    return {};
}
    
Result CppGenerator::generateFuncDeclare(ofstream& h, MetaFunc* metaFunc)
{
    CppFunc* cppFunc = CppFunc::getCppFunc(metaFunc);

    h << KEY_TAB;
    if (metaFunc->isVirtual)
    {
        h << "virtual ";
    }
    else if (metaFunc->isStatic)
    {
        h << "static ";
    }

    h << generateVarDefType(metaFunc->returnType) << " " << cppFunc->cppName << "(";

    for (auto& var : metaFunc->params)
    {
        if (var != metaFunc->params.front())
        {
            h << ", ";
        }
        h << generateFuncParamType(var->type) << " " << var->name;
    }

    if (metaFunc->block == nullptr)
    {
        h << ") = 0;" << endl;
    }
    else
    {
        h << ");" << endl;
    }

    if (metaFunc->funcType == FUNC_DESTRUCT)
    {
        //添加一个清除变量的接口
        h << KEY_TAB << "virtual OneVoid " << KEY_CLEAR_VAR_FUNC << "();" << endl;
    }

    return {};
}

Result CppGenerator::generateFuncImpl(ofstream& f, const string& space, MetaFunc* metaFunc, bool classPrefix)
{
    if (metaFunc->block == nullptr)
    {
        return {};
    }

    CppFunc* cppFunc = CppFunc::getCppFunc(metaFunc);

    MetaClass* metaClass = metaFunc->getOuterClass();
    CppClass* cppClass = CppClass::getCppClass(metaClass);

    MetaClass* metaParentClass = metaClass->getParentClass();
    CppClass* cppParentClass = CppClass::getCppClass(metaParentClass);

    if (classPrefix)
    {
        f << space << generateVarDefType(metaFunc->returnType) << " " << cppClass->cppName << "::" << cppFunc->cppName << "(";
    }
    else
    {
        f << space << generateVarDefType(metaFunc->returnType) << " " << cppFunc->cppName << "(";
    }
    for (auto& var : metaFunc->params)
    {
        if (var != metaFunc->params.front())
        {
            f << ", ";
        }

        f << generateFuncParamType(var->type) << " " << var->name;
    }
    f << ")" << endl;
    f << space << "{" << endl;

    if (metaFunc->funcType == FUNC_INIT)
    {
        //调用父类的Init
        if (cppParentClass->cppNative == false)
        {
            f << space << KEY_TAB << cppParentClass->cppName << "::" << KEY_INIT_VAR_FUNC << "();" << endl;
        }
    }
    else if (metaFunc->funcType == FUNC_CONSTRUCT)
    {
        //判断有没有手动调用其他构造函数的，如果没有调用父类的默认构造函数
        if (cppParentClass->cppNative == false)
        {
            bool have = false;
            for (auto& instruct : metaFunc->block->instructs)
            {
                if (instruct->cmd == CALL_FIXED 
                    && instruct->func->funcType == FUNC_CONSTRUCT 
                    && instruct->params.front() == MetaData(metaClass->getThisVariable()))
                {
                    have = true;
                    break;
                }
            }
            if (have == false)
            {
                //调用默认构造
                f << space << KEY_TAB << cppParentClass->cppName << "::" << KEY_CONSTRUCT_FUNC << "();" << endl;
            }
        }
    }

    //定义方法实现
    VR(generateBlock(f, space + KEY_TAB, metaFunc->block));
    
    if (metaFunc->funcType == FUNC_DESTRUCT)
    {
        //调用父类的析构函数
        if (cppParentClass->cppNative == false)
        {
            f << space << KEY_TAB << cppParentClass->cppName << "::" << KEY_DESTRUCT_FUNC << "();" << endl;
        }
    }

    f << space << "}" << endl << endl;

    if (metaFunc->funcType == FUNC_DESTRUCT)
    {
        f << space << "OneVoid " << cppClass->cppName << "::" << KEY_CLEAR_VAR_FUNC << "()" << endl;
        f << space << "{" << endl;
            
        for (auto& var : metaFunc->getOuterClass()->vars)
        {
            if (var->isStatic == false && var->type.isClass())
            {
                f << space << KEY_TAB << "this->" << var->name << ".clear();" << endl;
            }
        }

        //调用父类的析构函数
        if (cppParentClass->cppNative == false)
        {
            f << space << KEY_TAB << cppParentClass->cppName << "::" << KEY_CLEAR_VAR_FUNC << "();" << endl;
        }

        f << space << "}" << endl << endl;
    }

    return {};
}
    
string CppGenerator::generateType(const MetaType& type)
{
    if (type.isBaseType())
    {
        return MetaType::typeToCppString(type.type);
    }
    else
    {
        return CppClass::getCppClass(type.clazz)->cppName;
    }
}
    
string CppGenerator::generateVarDefType(const MetaType& type)
{
    if (type.isBaseType())
    {
        return MetaType::typeToCppString(type.type);
    }
    else
    {
        return "Reference<" + CppClass::getCppClass(type.clazz)->cppName + ">";
    }
}

string CppGenerator::generateFuncParamType(const MetaType& type)
{
    if (type.isBaseType())
    {
        return MetaType::typeToCppString(type.type);
    }
    else
    {
        return "Pointer<" + CppClass::getCppClass(type.clazz)->cppName + ">";
    }
}

void CppGenerator::addNativeClassImpl(const string& oneClassPath, const string& hPath, const string& cppClass)
{
    MetaClass* metaClass = metaContainer->getClass(oneClassPath);
    if (metaClass == nullptr)
    {
        return;
    }
    CppClass* cppClass1 = CppClass::getCppClass(metaClass, &cppContainer);
    cppClass1->cppNative = true;
    cppClass1->cppNativeName = cppClass;
    cppClass1->cppNativeHPath = hPath;

    for (auto& realMetaClass : metaClass->realClasses)
    {
        CppClass* cppClass2 = CppClass::getCppClass(realMetaClass, &cppContainer);
        cppClass2->cppNative = true;
        cppClass2->cppNativeName = cppClass;
        cppClass2->cppNativeHPath = hPath;
    }
    return;
}
    
string CppGenerator::generateData(MetaData& data)
{
    switch (data.type)
    {
    case MetaData::CONST:
        {
            MetaConst* const_ = data.const_;
            switch (const_->type)
            {
            case DT_BOOL:
                {
                    return const_->bvalue ? "true" : "false";
                }
                break;
            case DT_CHAR:
                {
                    return StringUtils::itoa(const_->cvalue);
                    // char c[] = "\'a\'";
                    // c[1] = const_->cvalue;
                    // return c;
                }
                break;
            case DT_SHORT:
                {
                    if (const_->remark.empty() == false)
                    {
                        return const_->remark;
                    }
                    else
                    {
                        return StringUtils::itoa(const_->svalue);
                    }
                }
                break;
            case DT_INT:
                {
                    if (const_->remark.empty() == false)
                    {
                        return const_->remark;
                    }
                    else
                    {
                        return StringUtils::itoa(const_->ivalue);
                    }
                }
                break;
            case DT_LONG:
                {
                    if (const_->remark.empty() == false)
                    {
                        return const_->remark;
                    }
                    else
                    {
                        return StringUtils::ltoa(const_->lvalue);
                    }
                }
                break;
            case DT_FLOAT:
                {
                    if (const_->remark.empty() == false)
                    {
                        return const_->remark;
                    }
                    else
                    {
                        return StringUtils::ftoa(const_->fvalue);
                    }
                }
                break;
            case DT_DOUBLE:
                {
                    if (const_->remark.empty() == false)
                    {
                        return const_->remark;
                    }
                    else
                    {
                        return StringUtils::dtoa(const_->dvalue);
                    }
                }
                break;
            case DT_STRING:
                {
                    return "g_stringPool.getString(" + StringUtils::itoa(getStringIndex(const_->remark)) + ")";
                }
                break;
            case DT_NULL:
                {
                    return "nullptr";     //对象和基础类型都能用
                }
                break;
            default:
                break;
            }
        }
        break;
    case MetaData::VARREF:
        {
            MetaVarRef* varRef = data.varRef;
            string cppData = generateData(varRef->obj);
            MetaType type = varRef->obj.getType();
            for (auto& member : varRef->members)
            {
                if (member->varType == VAR_THIS)
                {
                    //指的是自己，不需要任何变化
                    //cppData = "Pointer<" + generateType(index.var->type) + ">(" + cppData + ", true)";
                }
                else if (member->varType == VAR_SUPER)
                {
                    //转换成父类型
                    //cppData = "Pointer<" + generateType(index.var->type) + ">(" + cppData + ", true)";
                    cppData = "convertPointerForce<" + generateType(type) + ", " + generateType(member->type) + ">(" + cppData + ")";
                }
                else if (member->varType == VAR_CLASS)
                {
                    cppData = "Pointer<Class>(ClassP<" + member->box->convertClass()->name + ">::getClass(), true)";
                }
                else
                {
                    //成员变量
                    MetaClass* varInClass = (MetaClass*)member->box;
                    if (type.clazz == varInClass)
                    {
                        cppData = cppData + "->" + member->name;
                    }
                    else
                    {
                        cppData = cppData + "->" + CppClass::getCppClass(varInClass)->cppName + "::" + member->name;
                    }
                }
                type = member->type;
            }
            return cppData;
        }
        break;
    case MetaData::INSTRUCT:
        {
            MetaInstruct* instruct = data.instructTmp;
            //return "(" + std::move(instruct->cppCode) + ")";
            return std::move(instruct->cppCode);
        }
        break;
    case MetaData::VAR:
        {
            MetaVariable* var = data.var;
            if (var->varType == VAR_THIS)
            {
                return "Pointer<" + generateType(var->type) + ">(this, true)";
            }
            else if (var->varType == VAR_SUPER)
            {
                return "Pointer<" + generateType(var->type) + ">(this, true)";
            }
            else if (var->varType == VAR_CLASS)
            {
                return "Pointer<Class>(ClassP<" + var->box->convertClass()->name + ">::getClass(), true)";
            }
            else if (var->varType == VAR_MEMBER || var->varType == VAR_ANONY_THIS)
            {
                if (var->isStatic)
                {
                    return generateType(var->type) + "::" + var->name;
                }
                else
                {
                    return "this->" + var->name;
                }
            }
            else if (var->varType == VAR_PARAM)
            {
                return var->name;
            }
            else
            {
                return var->name;
            }
        }
        break;
    default:
        break;
    }
    return "";
}
    
string CppGenerator::generateTypeData(const string& str, const MetaType& dataType, const MetaType& type, bool pointer)
{
    if (dataType == type)
    {
        return str;
    }

    int convertType = metaContainer->getAutoConvertType(dataType, type);
    switch (convertType)
    {
    case MetaContainer::ACT_CANNT:
        {
            assert(false);
            return "";
        }
        break;
    case MetaContainer::ACT_EQUAL:
        {
            return str;
        }
        break;
    case MetaContainer::ACT_NULL:
        {
            if (type.isBaseType())
            {
                //如果是基础类型，转换成0
                return "0";
            }
            return str;
        }
        break;
    case MetaContainer::ACT_BASE_TYPE:
        {
            return "(" + generateType(type) + ")" + str;
        }
        break;
    case MetaContainer::ACT_PARENT_TYPE:
        {
            if (pointer)
            {
                return "convertPointer<" + generateType(dataType) + ", " + generateType(type) + ">(" + str + ")";
            }
            else if (type.clazz->isInterface)
            {
                return "convertInterfaceReference<" + generateType(dataType) + ", " + generateType(type) + ">(" + str + ")";
            }
            else
            {
                return "convertObjectReference<" + generateType(dataType) + ", " + generateType(type) + ">(" + str + ")";
            }
        }
        break;
    case MetaContainer::ACT_TEMPLATE:
        {
            if (pointer)
            {
                return "convertPointerForce<" + generateType(dataType) + ", " + generateType(type) + ">(" + str + ")";
            }
            else
            {
                return "convertReferenceForce<" + generateType(dataType) + ", " + generateType(type) + ">(" + str + ")";
            }
        }
        break;
    case MetaContainer::ACT_CONSTRUCT:
        {
            CppClass* cppClass = CppClass::getCppClass(type.clazz);
            return cppClass->getFactoryName() + "::" KEY_CREATE_OBJECT_FUNC "(" + str + ")";
        }
        break;
    case MetaContainer::ACT_VALUEOF:
        {
            //支持嵌套多级转换
            MetaFunc* func = metaContainer->searchClassFunction2(type.clazz, KEY_VALUEOF_FUNC, {dataType}, MFT_ONLY_STATIC);
            assert(func != nullptr);
            CppClass* cppClass = CppClass::getCppClass(type.clazz);
            return cppClass->cppName + "::" KEY_VALUEOF_FUNC "(" + generateTypeData(str, dataType, func->params.front()->type, false) + ")";
        }
        break;
    case MetaContainer::ACT_VALUE:
        {
            return str + "->" KEY_VALUE_FUNC "()";
        }
        break;
    default:
        break;
    }
    return str;
}
    
string CppGenerator::generateTypeData(MetaData& data, const MetaType& type, bool pointer)
{
    string str = generateData(data);
    MetaType dataType = data.getType();
    return generateTypeData(str, dataType, type, pointer);
}

int CppGenerator::getStringIndex(const string& str)
{
    auto iter = stringMap.find(str);
    if (iter != stringMap.end())
    {
        return iter->second;
    }
    int index = (int)stringMap.size();
    stringMap.insert({str, index});
    return index;
}
    
unsigned int CppGenerator::getUniqueId()
{
    return ++uniqueIdSeed;
}
