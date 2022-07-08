#pragma once
#include "OneObject.h"

namespace One
{
    template<class T>
    class Array : public Object
    {
    public:
        typename TemplateType<T>::VarType& operator [] (int index)
        {
            if (index < 0 || index >= _length)
            {
                //抛出异常
                throw "OutOfArray";
            }
            else
            {
                return _data[index];
            }
        }

        
        int length()
        {
            return _length;
        }

    public:
        unsigned int _length = 0;
        typename TemplateType<T>::VarType _data[1];
    };
}

