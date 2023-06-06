#pragma once
#include "OneObject.h"
#include "OneIterator.h"
#include "OneIterable.h"
#include "ObjectPool.h"
#include "ExceptionHelper.h"

namespace One
{
    template<class T> class ArrayIterator;

    ///////////////////////////////////////////////////////////////////////////////////
    // 支持C++11 RANGEFOR
    template<class T>
    class CppArrayIterator
    {
    public:
        CppArrayIterator(typename TemplateType<T>::VarType* data)
        {
            _data = data;
        }

        bool operator != (const CppArrayIterator& other) const
        {
            return _data != other._data;
        }

        typename TemplateType<T>::VarType& operator * () const
        {
            return *_data;
        }

        const CppArrayIterator& operator ++ ()
        {
            ++_data;
            return *this;
        }

    protected:
        typename TemplateType<T>::VarType* _data;
    };

    ///////////////////////////////////////////////////////////////////////////////////

    template<class T>
    class Array : public Object, public Iterable<T>
    {
        friend class ArrayIterator<T>;
    public:
        Array() : Iterable<T>(this)
        {

        }

        OneInt length()
        {
            return _length;
        }

    //其他操作符重载
    public:
        Reference<Array<T>> clone();
        Reference<Array<T>> combine(Array<Array<T>>* args);

    //[]括号操作符
    public:
        typename TemplateType<T>::VarType get(OneInt index);
        typename TemplateType<T>::VarType set(OneInt index, typename TemplateType<T>::FuncParamType value);

    //支持One的foreach
    public:
        virtual Reference<Iterator<T>> iterator();

    //C++ rangefor支持
    public:
        CppArrayIterator<T> begin() const
        {
            return CppArrayIterator<T>((typename TemplateType<T>::VarType*)_data);
        }

        CppArrayIterator<T> end() const
        {
            return CppArrayIterator<T>((typename TemplateType<T>::VarType*)_data + _length);
        }

    public:
        virtual void __destruct__()
        {
            _length = 0;
        }

        static Reference<Array<T>> createArray(OneInt length);
        static Reference<Array<T>> createArray(std::initializer_list<typename TemplateType<T>::VarType> init);

    protected:
        OneInt _length = 0;
        typename TemplateType<T>::VarType _data[1];
    };

    ////////////////////////////////////////////////////////////////////////////////////////
    //支持ONE FOREACH
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

        virtual OneBool hasNext()
        {
            return _data < (_arrRef->_data + _arrRef->_length);
        }

        virtual typename TemplateType<T>::VarType next()
        {
            return *(_data++);
        }

    protected:
        Reference<Array<T>> _arrRef;
        typename TemplateType<T>::VarType* _data = nullptr;
    };
}

#include "OneArray.inl"
