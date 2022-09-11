#pragma once
#include "MetaPackage.h"
#include "MetaClass.h"

namespace One
{
    class IByteStream
    {
    public:
        IByteStream()
        {
        }

        void load(const void* data, unsigned int length)
        {
            _data = (const char*)data;
            _length = length;
            _readPos = 0;
        }

        IByteStream& operator >> (string& str)
        {
            int length;
            *this >> length;
            str.resize(length);
            get((void*)str.data(), length);
            return *this;
        }
        IByteStream& operator >> (int& i)
        {
            get(&i, sizeof(i));
            return *this;
        }

    protected:
        void get(void* data, unsigned int len)
        {
            memcpy(data, _data + _readPos, len);
            _readPos += len;
        }

    protected:
        const char* _data = nullptr;
        unsigned int _readPos = 0;
        unsigned int _length = 0;
    };

    class MetaContainer
    {
    public:
        MetaContainer();

    public:
        void load(const void* data, unsigned int length);

        MetaClass* getClass(int id);

    protected:
        void loadPackage(MetaPackage* package);
        void loadClass(MetaClass* clazz);

    protected:
        IByteStream _stream;

        MetaPackage* _root;
        vector<MetaClass*> _classes;
    };

    extern MetaContainer g_metaContainer;
}
