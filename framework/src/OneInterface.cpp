#include "OneInterface.h"
#include "OneObject.h"

namespace One
{
    Interface::Interface(Object* obj)
    {
        this->_obj = obj;
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
