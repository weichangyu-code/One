#include "Dumper.h"
#include <sstream>
#include <fstream>
#include "../cpp/CppPackage.h"
#include "../../explainer/meta/MetaPackage.h"
#include "../../explainer/meta/MetaClass.h"
#include "../../explainer/meta/MetaContainer.h"
#include "../../explainer/meta/MetaFunc.h"
#include "../../explainer/meta/MetaVariable.h"

Result Dumper::dump(MetaContainer* container, const string& path)
{
    _stream << (int)container->getClasses().size();

    //设置包名
    VR(dumpPackage(container->getRootPackage()));
    
    VR(toCppFile(path));

    return {};
}
    
Result Dumper::dumpPackage(MetaPackage* package)
{
    _stream << package->name;
    _stream << (int)package->packages.size();
    for (auto& subPackage : package->packages)
    {
        VR(dumpPackage(subPackage));
    }
    _stream << (int)package->classes.size();
    for (auto& clazz : package->classes)
    {
        VR(dumpClass(clazz));
    }
    return {};
}
    
Result Dumper::dumpClass(MetaClass* clazz)
{
    _stream << clazz->id;
    _stream << clazz->name;

    _stream << (int)clazz->parents.size();
    for (auto& parent : clazz->parents)
    {
        _stream << parent->id;
    }

    _stream << (int)clazz->innerClasses.size();
    for (auto& inner : clazz->innerClasses)
    {
        VR(dumpClass(inner));
    }

    int funcNum = 0;
    for (auto& func : clazz->funcs)
    {
        funcNum++;
    }
    _stream << funcNum;
    for (auto& func : clazz->funcs)
    {
        VR(dumpFunc(func));
    }

    _stream << (int)clazz->vars.size();
    for (auto& var : clazz->vars)
    {
        VR(dumpField(var));
    }

    return {};
}

Result Dumper::dumpFunc(MetaFunc* func)
{
    _stream << func->name;

    _stream << (int)func->anonyClasses.size();
    for (auto& anony : func->anonyClasses)
    {
        VR(dumpClass(anony));
    }

    return {};
}
    
Result Dumper::dumpField(MetaVariable* var)
{
    _stream << var->name;
    return {};
}
    
Result Dumper::toCppFile(const string& path)
{
    const unsigned int* data = (unsigned int*)_stream.getData();
    unsigned int num = (_stream.getLength() + 3) / sizeof(unsigned int);

    ofstream f(path);
    if (f.is_open() == false)
    {
        return R_FAILED;
    }

    f << "unsigned int oneMeta[] = {";
    for (int i = 0;i < (int)num;i++)
    {
        char buf[16] = {0};
        sprintf(buf, "0x%x", data[i]);
        f << buf << ", ";
    }
    f << "0x0};\n\n";

    return {};
}
