#pragma once
#include "OneObject.h"
#include "OneIterator.h"
#include "OneIterable.h"
#include "ObjectPool.h"

namespace One
{
    template<class T> class ArrayIterator;

    template<class T>
    class Array : public Object, public Iterable<T>
    {
        friend class ArrayIterator<T>;
    public:
        Array() : Iterable<T>(this)
        {

        }

        typename TemplateType<T>::VarType& operator [] (int index)
        {
            return at(index);
        }

        typename TemplateType<T>::VarType& at(int index)
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
        virtual void __destruct__()
        {
            _length = 0;
        }

        static Reference<Array<T>> createArray(unsigned int length)
        {
            Array<T>* arr = g_objectPool.createObjectT<Array<T>>(sizeof(Array<T>) + length * sizeof(Array<T>::_data));
            arr->_length = length;
            return Reference<Array<T>>(arr, false, false);
        }

        static Reference<Array<T>> createArray(std::initializer_list<typename TemplateType<T>::VarType> init)
        {
            Reference<Array<T>> arrRef = createArray(init.size());
            int i = 0;
            for (auto& v : init)
            {
                arrRef->_data[i] = v;
                i++;
            }
            return arrRef;
        }

    protected:
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

        virtual typename TemplateType<T>::VarType& next()
        {
            return *(_data++);
        }

    public:
        Reference<Array<T>> _arrRef;
        typename TemplateType<T>::VarType* _data = nullptr;
    };

    template<class T>
    Reference<Iterator<T>> Array<T>::iterator()
    {
        Reference<ArrayIterator<T>> iterRef = g_objectPool.createObjectR<ArrayIterator<T>>();
        iterRef->setData(this);
        return convertInterfaceReference<ArrayIterator<T>, Iterator<T>>(iterRef);
    }
}

