#include "CppGenerator.h"
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
#include "StringUtils.h"
#include "../explainer/common/Keyword.h"
#include <sstream>
#include <fstream>
#include "cpp/CppPackage.h"
#include "cpp/CppFunc.h"
#include "cpp/CppClass.h"
#include "cpp/CppFile.h"
using namespace OneCommon;

CppGenerator::CppGenerator(MetaContainer* metaContainer)
{
    this->metaContainer = metaContainer;
}
    
CppGenerator::~CppGenerator()
{
    
}
    
Result CppGenerator::generate(const string& exeName, const string& mainClass, const string& depend, const string& folder)
{
    //避免误删的情况
    FileUtils::remove(FileUtils::appendFileName(folder, "one"));

    //添加原生类
    VR(addNativeClass("one.std.Object", "OneObject.h", "Object"));
    VR(addNativeClass("one.std.String", "OneString.h", "String"));
    VR(addNativeClass("one.std.Out", "OneOut.h", "Out"));
    VR(addNativeClass("one.std.Array", "OneArray.h", "Array"));
    VR(addNativeClass("one.std.System", "OneSystem.h", "System"));
    VR(addNativeClass("one.std.CoRunner", "OneSystem.h", "CoRunner"));

    MetaPackage* rootPackage = metaContainer->getRootPackage();

    generateCppInfoPackage("", "", rootPackage);
    generateCppAllClassName();

    VR(generatePackage(folder, rootPackage));

    VR(generateMainFile(folder, mainClass));

    VR(generateCMakeList(folder, exeName, depend));

    return {};
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
    MetaFunc* metaFunc = metaClass->getFunction("main", false);
    if (metaFunc == nullptr)
    {
        return R_FAILED;
    }
    auto cppClass = CppClass::getCppClass(metaClass);
    auto cppFunc = CppFunc::getCppFunc(metaFunc);

    cpp << "#include \"" << cppClass->cppHPath << "\"" << endl;
    cpp << "#include \"" << "StringPool.h" << "\"" << endl;
    for (auto& iterMetaClass : metaContainer->getClasses())
    {
        auto iterCppClass = CppClass::getCppClass(iterMetaClass);
        if (iterMetaClass->isInterface == false 
            && iterCppClass->cppNative == false
            && iterMetaClass->isTemplateClass() == false)
        {
            cpp << "#include \"" << iterCppClass->cppHPath << "\"" << endl;
        }
    }
    cpp << "#include \"" << "engine/Engine.h" << "\"" << endl;

    cpp << endl;
    cpp << "int main()" << endl;
    cpp << "{" << endl;

    //插入字符串
    cpp << "    One::g_stringPool.reserve(" << stringMap.size() << ");" << endl;
    for (auto& pair : stringMap)
    {
        cpp << "    One::g_stringPool.addString(" << pair.second << ", \"" << pair.first << "\");" << endl;
    }
    cpp << endl;

    cpp << "    int ret = 0;" << endl;
    cpp << "    OneCoroutine::Engine engine;" << endl;
    cpp << "    engine.createCoroutine([&ret](OneCoroutine::Coroutine* co) {" << endl;

    //调用静态初始化函数
    set<MetaClass*> called;
    for (auto& iterMetaClass : metaContainer->getClasses())
    {
        generateMainInitStateVar(cpp, called, iterMetaClass);
    }
    cpp << endl;

    
    cpp << "        auto app = One::" << cppClass->getFactoryName() << "::" << KEY_CREATE_OBJECT_FUNC << "();" << endl;
    if (metaFunc->return_.isInteger())
    {
        cpp << "        ret = app->main();" << endl;
    }
    else
    {
        cpp << "        app->main();" << endl;
    }
    cpp << "    });" << endl;
    cpp << "    engine.run();" << endl;
    cpp << "    return ret;" << endl;

    cpp << "}" << endl;

    return {};
}
    
void CppGenerator::generateMainInitStateVar(ofstream& f, set<MetaClass*>& called, MetaClass* metaClass)
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
        generateMainInitStateVar(f, called, parent);
    }

    f << "        One::" << cppClass->cppName << "::" << KEY_INIT_STATIC_VAR_FUNC << "();" << endl;
}

Result CppGenerator::generateCMakeList(const string& root, const string& exeName, const string& depend)
{
    ofstream f(FileUtils::appendFileName(root, "CMakeLists.txt"));
    if (f.is_open() == false)
    {
        return R_FAILED;
    }

    f << "cmake_minimum_required(VERSION 3.0.0)" << endl;
    f << "project(" << exeName << " VERSION 0.1.0)" << endl;
    f << "add_definitions(-std=c++11)" << endl;

    f << "include_directories(./ " << FileUtils::appendFileName(depend, "framework/src") << " " << FileUtils::appendFileName(depend, "coroutine/src") << ")" << endl;

    for (auto& metaClass : metaContainer->getClasses())
    {
        auto cppClass = CppClass::getCppClass(metaClass);
        if (metaClass->isInterface == false 
            && cppClass->cppNative == false 
            && metaClass->isTemplateClass() == false)
        {
            f << "set(SRC_LIST ${SRC_LIST} " << cppClass->cppPath << ")" << endl;
        }
    }
    f << "set(SRC_LIST ${SRC_LIST} main.cpp)" << endl;

    f << "add_executable(" << exeName << " ${SRC_LIST})" << endl;
    f << "target_link_libraries(" << exeName << " framework engine socket coctx common)" << endl;
    f << "if(WIN32)" << endl;
    f << "  target_link_libraries(" << exeName << " wsock32 ws2_32)" << endl;
    f << "endif()" << endl;

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
        generateCppInfoClass(path, namePrefix + StringUtils::toFirstUpper(metaClass->name), inner);
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
    FileUtils::createDir(FileUtils::appendFileName(root, CppPackage::getCppPackage(metaPackage)->cppFolder));

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

    //只创建对象工厂
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

    //
    set<string> includes;
    includes.insert(cppClass->cppNativeHPath);
    for (auto& parent : metaClass->parents)
    {
        includes.insert(CppClass::getCppClass(parent)->cppHPath);
    }
    set<string> classNames;
    for (auto& link : metaClass->linkClasses)
    {
        CppClass* cppLink = CppClass::getCppClass(link);
        if (cppLink->cppNative)
        {
            //因为cppNative用typedef定义的，不能用class声明
            includes.insert(cppLink->cppHPath);
        }
        else
        {
            classNames.insert(cppLink->cppName);
        }
    }

    //头文件包含
    h << "#pragma once" << endl;
    h << "#include \"ObjectPool.h\"" << endl;
    for (auto& name : includes)
    {
        h << "#include \"" << name << "\"" << endl;
    }
    h << endl;

    h << "namespace One {" << endl << endl;
    
    //声明类
    for (auto& name : classNames)
    {
        h << "class " << name << ";" << endl;
    }

    //类型转换
    if (metaClass->isTemplateClass())
    {
        h << "typedef " << cppClass->cppNativeName << " " << cppClass->cppName << ";" << endl;
    }
    else
    {
        h << "typedef " << cppClass->cppNativeName << cppClass->getTemplateParamImpl() << " " << cppClass->cppName << ";" << endl;
    }

    VR(generateFactoryClass(h, metaClass));

    h << "}" << endl;

    return {};
}

Result CppGenerator::generateFactoryClass(ofstream& f, MetaClass* metaClass)
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
        f << ")" << endl;
        
        f << KEY_TAB << "{" << endl;
        
        f << KEY_TAB << KEY_TAB << "auto __var__ = (" << cppClass->cppName << "*)g_objectPool.createObject(sizeof(" << cppClass->cppName << "));" << endl;
        if (cppClass->cppNative)
        {
            f << KEY_TAB << KEY_TAB << "CALL_CONSTRUCT(__var__, " << cppClass->cppName;
            for (auto& var : metaFunc->params)
            {
                f << ", ";
                f << var->name;
            }
            f << ");" << endl;
        }
        else
        {
            f << KEY_TAB << KEY_TAB << "CALL_CONSTRUCT(__var__, " << cppClass->cppName << ");" << endl;
            f << KEY_TAB << KEY_TAB << "__var__->" << KEY_INIT_VAR_FUNC << "();" << endl;
            f << KEY_TAB << KEY_TAB << "__var__->" << KEY_CONSTRUCT_FUNC << "(";
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
        f << KEY_TAB << KEY_TAB << "return Reference<" << cppClass->cppName << ">(__var__, false, false);" << endl;

        f << KEY_TAB << "}" << endl << endl;
    }

    f << "};" << endl << endl;

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
    set<string> classNames;
    for (auto& link : metaClass->linkClasses)
    {
        CppClass* cppLink = CppClass::getCppClass(link);
        if (cppLink->cppNative)
        {
            //TODO:这个为什么
            includes.insert(cppLink->cppHPath);
        }
        else
        {
            classNames.insert(cppLink->cppName);
        }
    }

    //头文件包含
    h << "#pragma once" << endl;
    h << "#include \"" << "OneInterface.h" << "\"" << endl;
    for (auto& name : includes)
    {
        h << "#include \"" << name << "\"" << endl;
    }
    h << endl;

    h << "namespace One {" << endl << endl;

    //声明类
    for (auto& name : classNames)
    {
        h << "class " << name << ";" << endl;
    }

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
    else
    {
        h << " : public Interface";
    }
    h << endl;
    h << "{" << endl;
    h << "public:" << endl;

    //构造函数
    h << KEY_TAB << cppClass->cppName << "(Object* obj) : ";
    if (metaClass->parents.empty() == false)
    {
        for (auto& parent : metaClass->parents)
        {
            if (parent != metaClass->parents.front())
            {
                h << ", ";
            }
            h << CppClass::getCppClass(parent)->cppName << "(obj)";
        }
    }
    else
    {
        h << "Interface(obj)";
    }
    h << " {}" << endl;

    //方法
    for (auto& metaFunc : metaClass->funcs)
    {
        VR(generateFuncDeclare(h, metaFunc));
    }

    h << "};" << endl << endl;
    h << "}" << endl << endl;

    return {};
}
    
Result CppGenerator::generateNativeInterface(const string& root, MetaClass* metaClass)
{
    if (metaClass->isInterface == false)
    {
        return {};
    }

    //只创建对象工厂
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

    //
    set<string> includes;
    includes.insert(cppClass->cppNativeHPath);
    for (auto& parent : metaClass->parents)
    {
        includes.insert(CppClass::getCppClass(parent)->cppHPath);
    }
    set<string> classNames;
    for (auto& link : metaClass->linkClasses)
    {
        CppClass* cppLink = CppClass::getCppClass(link);
        if (cppLink->cppNative)
        {
            //因为cppNative用typedef定义的，不能用class声明
            includes.insert(cppLink->cppHPath);
        }
        else
        {
            classNames.insert(cppLink->cppName);
        }
    }

    //头文件包含
    h << "#pragma once" << endl;
    h << "#include \"ObjectPool.h\"" << endl;
    for (auto& name : includes)
    {
        h << "#include \"" << name << "\"" << endl;
    }
    h << endl;

    h << "namespace One {" << endl << endl;
    
    //声明类
    for (auto& name : classNames)
    {
        h << "class " << name << ";" << endl;
    }

    //类型转换
    if (metaClass->isTemplateClass())
    {
        h << "typedef " << cppClass->cppNativeName << " " << cppClass->cppName << ";" << endl;
    }
    else
    {
        h << "typedef " << cppClass->cppNativeName << cppClass->getTemplateParamImpl() << " " << cppClass->cppName << ";" << endl;
    }

    h << "}" << endl;

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

    //
    set<string> includes;
    for (auto& parent : metaClass->parents)
    {
        includes.insert(CppClass::getCppClass(parent)->cppHPath);
    }
    set<string> classNames;
    for (auto& link : metaClass->linkClasses)
    {
        CppClass* cppLink = CppClass::getCppClass(link);
        if (cppLink->cppNative)
        {
            includes.insert(cppLink->cppHPath);
        }
        else
        {
            classNames.insert(cppLink->cppName);
        }
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
    for (auto& name : classNames)
    {
        h << "class " << name << ";" << endl;
    }

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
    h << KEY_TAB << cppClass->cppName << "()";
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
                h << " : ";
            }
            else
            {
                h << ", ";
            }
            h << CppClass::getCppClass(parent)->cppName << "(this)";

            index++;
        }
    }
    h << " {}" << endl;

    //变量
    for (auto& var : metaClass->vars)
    {
        h << KEY_TAB;
        if (var->isStatic)
        {
            h << "static ";
        }
        if (var->isConst)
        {
            h << "const ";
        }

        h << generateVarDefType(var->type) << " " << var->name << ";" << endl;
    }
    h << endl;

    //加上三个固定的方法
    if (metaClass->isInterface == false)
    {
        VR(generateFuncDeclare(h, metaClass->varInitFunc));
        VR(generateFuncDeclare(h, metaClass->staticVarInitFunc));
    }

    //方法
    for (auto& metaFunc : metaClass->funcs)
    {
        VR(generateFuncDeclare(h, metaFunc));
    }

    h << "};" << endl << endl;

    VR(generateFactoryClass(h, metaClass));

    h << "}" << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////
    //源文件
    if (metaClass->isInterface == false)
    {
        ofstream cpp(FileUtils::appendFileName(root, cppClass->cppPath));
        if (cpp.is_open() == false)
        {
            return R_FAILED;
        }

        cpp << "#include \"" << FileUtils::getFileName(cppClass->cppHPath) << "\"" << endl;
        cpp << "#include \"" << "StringPool.h" << "\"" << endl;
        cpp << "#include \"" << "ObjectPool.h" << "\"" << endl;
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

        for (auto& var : metaClass->vars)
        {
            if (var->isStatic)
            {
                cpp << generateVarDefType(var->type) << " "  << cppClass->cppName << "::" << var->name << ";" << endl;
            }
        }
        cpp << endl;

        VR(generateFuncImpl(cpp, "", metaClass->varInitFunc, true));
        VR(generateFuncImpl(cpp, "", metaClass->staticVarInitFunc, true));
        for (auto& metaFunc : metaClass->funcs)
        {
            VR(generateFuncImpl(cpp, "", metaFunc, true));
        }

        cpp << endl << "}" << endl;
    }

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

Result CppGenerator::generateBlock(ofstream& f, const string& space, MetaBlock* block)
{
    int labelIndex = 0;
    for (auto& instruct : block->instructs)
    {
        switch (instruct->cmd)
        {
        case INDEX:
            {
                instruct->cppCode = generateData(instruct->params.front());
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
                instruct->cppCode = generateData(instruct->params.front()) + " * " + generateData(instruct->params.back());
            }
            break;
        case DIV:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " / " + generateData(instruct->params.back());
            }
            break;
        case REC:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " % " + generateData(instruct->params.back());
            }
            break;
        case ADD:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " + " + generateData(instruct->params.back());
            }
            break;
        case SUB:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " - " + generateData(instruct->params.back());
            }
            break;
        case RBITMOV:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " >> " + generateData(instruct->params.back());
            }
            break;
        case RBITMOV2:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " >>> " + generateData(instruct->params.back());
            }
            break;
        case LBITMOV:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " << " + generateData(instruct->params.back());
            }
            break;
        case GT:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " > " + generateData(instruct->params.back());
            }
            break;
        case GTE:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " >= " + generateData(instruct->params.back());
            }
            break;
        case LT:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " < " + generateData(instruct->params.back());
            }
            break;
        case LTE:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " <= " + generateData(instruct->params.back());
            }
            break;
        case EQ:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " == " + generateData(instruct->params.back());
            }
            break;
        case NEQ:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " != " + generateData(instruct->params.back());
            }
            break;
        case EQ_DEEP:
        case NEQ_DEEP:
            {
                //TODO
            }
            break;
        case BITAND:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " & " + generateData(instruct->params.back());
            }
            break;
        case BITXOR:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " ^ " + generateData(instruct->params.back());
            }
            break;
        case BITOR:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " | " + generateData(instruct->params.back());
            }
            break;
        case AND:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " && " + generateData(instruct->params.back());
                instruct->cppCode = "(" + instruct->cppCode + ")";
            }
            break;
        case OR:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " || " + generateData(instruct->params.back());
            }
            break;
        case COND:
            {
                string param0 = generateData(instruct->params.front());
                string param1 = generateData((*(++instruct->params.begin())));
                string param2 = generateData(instruct->params.back());
                instruct->cppCode = param0 + " ? " + param1 + " : " + param2;
            }
            break;
        case ASSIGN:
            {
                instruct->cppCode = generateData(instruct->params.front()) + " = " + generateData(instruct->params.back());
            }
            break;
        case DEEP_ASSIGN:
            {
                //TODO:
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
        case RETURN:
            {
                if (instruct->params.empty())
                {
                    instruct->cppCode = "return";
                }
                else
                {
                    instruct->cppCode = "return " + generateData(instruct->params.front());
                }
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
                instruct->cppCode = "g_objectPool.createArray<" 
                    + generateType(instruct->retType.clazz->params.front()->type) 
                    + ">(" + generateData(instruct->params.front()) + ")";
            }
            break;
        case CALL:
        case CALL_FIXED:
            {
                ostringstream stream;

                MetaFunc* metaFunc = instruct->func;
                auto cppFunc = CppFunc::getCppFunc(metaFunc);
                auto cppClass = CppClass::getCppClass(metaFunc->getOuterClass());

                if (metaFunc->isStatic)
                {
                    //直接调用
                    stream << cppClass->cppName << "::" << cppFunc->cppName << "(";
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
                }
                else
                {
                    int index = 0;
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
                            stream << generateData(param);
                        }
                        else
                        {
                            stream << ", " << generateData(param);
                        }

                        index++;
                    } 
                    stream << ")";
                }
                instruct->cppCode = stream.str();
            }
            break;
        case VARDEF:
            {
                MetaVariable* var = instruct->var;
                instruct->cppCode = generateVarDefType(var->type) + " " + var->name;
            }
            break;
        }
    }

    for (auto& instruct : block->instructs)
    {
        if (instruct->cmd == BLOCK)
        {
            f << space << "{" << endl;
            VR(generateBlock(f, space + KEY_TAB, instruct->block));
            f << space << "}" << endl;
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

    h << generateVarDefType(metaFunc->return_) << " " << cppFunc->cppName << "(";

    for (auto& var : metaFunc->params)
    {
        if (var != metaFunc->params.front())
        {
            h << ", ";
        }
        h << generateFuncParamType(var->type) << " " << var->name;
    }

    h << ");" << endl;

    if (metaFunc->funcType == FUNC_DESTRUCT)
    {
        //添加一个清除变量的接口
        h << KEY_TAB << "void " << KEY_CLEAR_VAR_FUNC << "();" << endl;
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
        f << space << generateVarDefType(metaFunc->return_) << " " << cppClass->cppName << "::" << cppFunc->cppName << "(";
    }
    else
    {
        f << space << generateVarDefType(metaFunc->return_) << " " << cppFunc->cppName << "(";
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
        //变量释放
        f << space << KEY_TAB << KEY_CLEAR_VAR_FUNC << "();" << endl;

        //调用父类的析构函数
        if (cppParentClass->cppNative == false)
        {
            f << space << KEY_TAB << cppParentClass->cppName << "::" << KEY_DESTRUCT_FUNC << "();" << endl;
        }
    }

    f << space << "}" << endl << endl;

    if (metaFunc->funcType == FUNC_DESTRUCT)
    {
        f << space << "void " << cppClass->cppName << "::" << KEY_CLEAR_VAR_FUNC << "()" << endl;
        f << space << "{" << endl;
            
        for (auto& var : metaFunc->getOuterClass()->vars)
        {
            if (var->isStatic == false && var->type.isClass())
            {
                f << space << KEY_TAB << "this->" << var->name << ".clear();" << endl;
            }
        }
        f << space << "}" << endl << endl;
    }

    return {};
}
    
string CppGenerator::generateType(MetaType& type)
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
    
string CppGenerator::generateVarDefType(MetaType& type)
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
    
string CppGenerator::generateFuncParamType(MetaType& type)
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

Result CppGenerator::addNativeClass(const string& oneClassPath, const string& hPath, const string& cppClass)
{
    MetaClass* metaClass = metaContainer->getClass(oneClassPath);
    if (metaClass == nullptr)
    {
        return R_FAILED;
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
    return {};
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
                    char c[] = "\'a\'";
                    c[1] = const_->cvalue;
                    return c;
                }
                break;
            case DT_INT:
            case DT_LONG:
            case DT_FLOAT:
            case DT_DOUBLE:
                {
                    return const_->remark;
                }
                break;
            case DT_STRING:
                {
                    return "g_stringPool.getString(" + StringUtils::itoa(getStringIndex(const_->remark)) + ")";
                }
                break;
            case DT_CLASS:
                {
                    return "nullptr";
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
            for (auto& index : varRef->indexes)
            {
                if (index.type == MetaData::MEMBER)
                {
                    if (index.var->varType == VAR_THIS)
                    {
                        //转换成内部引用指针
                        cppData = "Pointer<" + generateType(index.var->type) + ">(" + cppData + ", true)";
                    }
                    else if (index.var->varType == VAR_SUPER)
                    {
                        cppData = "Pointer<" + generateType(index.var->type) + ">(" + cppData + ", true)";
                    }
                    else
                    {
                        //成员变量
                        MetaClass* varInClass = (MetaClass*)index.var->box;
                        if (type.clazz == varInClass)
                        {
                            cppData = cppData + "->" + index.var->name;
                        }
                        else
                        {
                            cppData = cppData + "->" + CppClass::getCppClass(varInClass)->cppName + "::" + index.var->name;
                        }
                    }
                    type = index.var->type;
                }
                else
                {
                    assert(metaContainer->isArray(type));
                    cppData = "(*" + cppData + ")[" + generateData(index) + "]";
                    type = type.clazz->params.front()->type;
                }
            }
            return cppData;
        }
        break;
    case MetaData::INSTRUCT:
        {
            MetaInstruct* instruct = data.instructTmp;
            return "(" + std::move(instruct->cppCode) + ")";
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
            else if (var->varType == VAR_MEMBER)
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
    return {};
}

int CppGenerator::getStringIndex(const string& str)
{
    auto iter = stringMap.find(str);
    if (iter != stringMap.end())
    {
        return iter->second;
    }
    int index = stringMap.size();
    stringMap.insert({str, index});
    return index;
}
