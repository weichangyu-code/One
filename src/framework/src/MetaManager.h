#pragma once
#include "OnePackage.h"
#include "OneClass.h"
#include "OneField.h"
#include "OneFunction.h"

namespace One
{
    class IByteStream
    {
    public:
        IByteStream();

        void load(const void* data, unsigned int length);

        IByteStream& operator >> (string& str);
        IByteStream& operator >> (int& i);

    protected:
        void get(void* data, unsigned int len);

    protected:
        const char* _data = nullptr;
        unsigned int _readPos = 0;
        unsigned int _length = 0;
    };

    class MetaManager
    {
    public:
        MetaManager();

    public:
        void load(const void* data, unsigned int length);

        Class* getClass(int id);

    protected:
        void loadPackage(Package* package);
        Class* loadClass();
        void loadFunction(Function* func);
        void loadField(Field* field);

    protected:
        IByteStream _stream;

        Package* _root;
        vector<Class*> _classes;
    };

    extern MetaManager g_metaManager;
}
