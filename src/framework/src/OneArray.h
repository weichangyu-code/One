#pragma once
#include "OneObject.h"
#include "OneIterator.h"
#include "OneIterable.h"
#include "ObjectPool.h"

namespace One
{
    template<class T>
    class Array : public Object, public Iterable<T>
    {
    public:
        Array() : Iterable<T>(this)
        {

        }

        static Reference<Array<T>> createArray(unsigned int length)
        {
            Array<T>* arr = g_objectPool.createObjectT<Array<T>>(sizeof(Array<T>) + length * sizeof(Array<T>::_data));
            arr->_length = length;
            return Reference<Array<T>>(arr, false, false);
        }

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

        virtual Reference<Iterator<T>> iterator();

    public:
        unsigned int _length = 0;
        typename TemplateType<T>::VarType _data[1];
    };

    template<class T>
    class ArrayIterator : public Object, public Iterator<T>
    {
    public:
        ArrayIterator() : Iterator<T>(this)
        {
        }

        void setData(Array<T>* arr)
        {
            _arrRef = arr;
            _data = arr->_data;
        }

        virtual bool hasNext()
        {
            return _data < (_arrRef->_data + _arrRef->_length);
        }

        virtual T next()
        {
            return *(_data++);
        }

    public:
        Reference<Array<T>> _arrRef;
        T* _data = nullptr;
    };

    template<class T>
    Reference<Iterator<T>> Array<T>::iterator()
    {
        Reference<ArrayIterator<T>> iterRef = g_objectPool.createObjectR<ArrayIterator<T>>();
        iterRef->setData(this);
        return convertInterfaceReference<ArrayIterator<T>, Iterator<T>>(iterRef);
    }
}

