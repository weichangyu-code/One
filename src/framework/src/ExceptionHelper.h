#pragma once
#include "OneException.h"

namespace One
{
    class ExceptionBox
    {
    public:
        ExceptionBox(const Reference<Exception>& e)
        {
            this->e = e;
        }
        ExceptionBox(const ExceptionBox& box)
        {
            this->e = box.e;
        }
        
    public:
        Reference<Exception> e;
    };

    void throwException(Exception* e);
    void throwNullPointerException();
    void throwOutOfArrayException();

}
