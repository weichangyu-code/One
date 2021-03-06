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

    void Interface::__acquireObj__(bool inner)
    {
        __obj__->__acquire__(inner);
    }
        
    void Interface::__releaseObj__(bool inner)
    {
        __obj__->__release__(inner);
    }
        
    void Interface::__destroyObj__()
    {
        __obj__->__destroy__();
    }
} // namespace One
