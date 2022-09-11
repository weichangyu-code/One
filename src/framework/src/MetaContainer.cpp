#include "MetaContainer.h"
#include <string>
using namespace std;

namespace One
{
    MetaContainer g_metaContainer;

    MetaContainer::MetaContainer()
    {

    }

    void MetaContainer::load(const void* data, unsigned int length)
    {
        _stream.load(data, length);

        int classNum;
        _stream >> classNum;
        _classes.resize(classNum + 1);

        //这个后续优化，可以从私人缓存里面分配，提高速度，减少内存占用
        _root = new MetaPackage(nullptr);
        loadPackage(_root);
    }
        
    MetaClass* MetaContainer::getClass(int id)
    {
        return _classes[id];
    }
        
    void MetaContainer::loadPackage(MetaPackage* package)
    {
        _stream >> package->name;

        int subNum;
        _stream >> subNum;
        for (int i = 0;i < subNum;i++)
        {
            MetaPackage* subPackage = new MetaPackage(package);
            package->packages.push_back(subPackage);
            loadPackage(subPackage);
        }

        int classNum;
        _stream >> classNum;
        for (int i = 0;i < classNum;i++)
        {
            MetaClass* clazz = new MetaClass(package);
            package->classes.push_back(clazz);
            loadClass(clazz);
        }
    }
        
    void MetaContainer::loadClass(MetaClass* clazz)
    {
        _stream >> clazz->id;
        _stream >> clazz->name;

        _classes[clazz->id] = clazz;
    }
}
