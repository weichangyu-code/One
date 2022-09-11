#include "Dumper.h"
#include <sstream>
#include <fstream>
#include "../cpp/CppPackage.h"
#include "../../explainer/meta/MetaPackage.h"
#include "../../explainer/meta/MetaClass.h"
#include "../../explainer/meta/MetaContainer.h"

Result Dumper::dump(MetaContainer* container, const string& path)
{
    _stream << container->getClasses().size();

    //设置包名
    VR(dumpPackage(container->getRootPackage()));
    
    VR(toCppFile(path));

    return {};
}
    
Result Dumper::dumpPackage(MetaPackage* package)
{
    _stream << package->name;
    _stream << package->packages.size();
    for (auto& subPackage : package->packages)
    {
        VR(dumpPackage(subPackage));
    }
    _stream << package->classes.size();
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
    for (int i = 0;i < num;i++)
    {
        char buf[16] = {0};
        sprintf(buf, "0x%x", data[i]);
        f << buf << ", ";
    }
    f << "0x0};\n\n";

    return {};
}
