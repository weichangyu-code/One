#include "Interface.h"
#include "OneObject.h"

namespace One
{
    Interface::Interface(Object* obj)
    {
        this->_obj = obj;
    }
    
    Interface::~Interface()
    {
        
    }

    void Interface::acquireObj(bool inner)
    {
        _obj->acquire(inner);
    }
        
    void Interface::releaseObj(bool inner)
    {
        _obj->release(inner);
    }
} // namespace One
