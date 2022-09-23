#include "OneInterface.h"
#include "OneObject.h"

namespace One
{
    Interface::Interface(Object* obj)
    {
        this->__obj__ = obj;
    }

    Interface::~Interface()
    {
        
    }

    void Interface::__acquireObj__(bool owner)
    {
        __obj__->__acquire__(owner);
    }
        
    void Interface::__releaseObj__(bool owner)
    {
        __obj__->__release__(owner);
    }
        
    void Interface::__destroyObj__()
    {
        __obj__->__destroy__();
    }
} // namespace One
