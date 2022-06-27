#pragma once

namespace One
{
    class MetaClass;
    class RunClass
    {
    public:
        RunClass(MetaClass* clazz);

    public:
        MetaClass* clazz = nullptr;
    };
}
