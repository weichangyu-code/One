
namespace One
{
    template<class T>
    Reference<Iterator<T>> Array<T>::iterator()
    {
        Reference<ArrayIterator<T>> iterRef = g_objectPool.createObjectR<ArrayIterator<T>>();
        iterRef->setData(this);
        return convertInterfaceReference<ArrayIterator<T>, Iterator<T>>(iterRef);
    }

    template<class T>
    Reference<Array<T>> Array<T>::createArray(OneInt length)
    {
        Array<T>* arr = g_objectPool.createObjectT<Array<T>>(sizeof(Array<T>) + length * sizeof(Array<T>::_data));
        arr->_length = length;
        return Reference<Array<T>>(arr, true, false);
    }

    template<class T>
    Reference<Array<T>> Array<T>::createArray(std::initializer_list<typename TemplateType<T>::VarType> init)
    {
        Reference<Array<T>> arrRef = createArray((unsigned int)init.size());
        typename TemplateType<T>::VarType* refData = arrRef->_data;
        for (auto& v : init)
        {
            *refData = v;
            refData++;
        }
        return arrRef;
    }
    
    template<class T>
    typename TemplateType<T>::VarType Array<T>::get(OneInt index)
    {
        if (index < 0 || index >= _length)
        {
            //定义温和的语言，轻易不抛异常
            //throwOutOfArrayException();
            return DefaultValue<T>::value();
        }
        else
        {
            return _data[index];
        }
    }

    template<class T>
    typename TemplateType<T>::VarType Array<T>::set(OneInt index, typename TemplateType<T>::FuncParamType value)
    {
        if (index < 0 || index >= _length)
        {
            return DefaultValue<T>::value();
        }
        else
        {
            _data[index] = value;
            return value;
        }
    }

    template<class T>
    Reference<Array<T>> Array<T>::clone()
    {
        Reference<Array<T>> ref = Array<T>::createArray(_length);
        typename TemplateType<T>::VarType* refData = ref->_data;
        for (int i = 0;i < _length;i++)
        {
            refData[i] = _data[i];
        }
        return ref;
    }
    
    template<class T>
    Reference<Array<T>> Array<T>::combine(Array<Array<T>>* args)
    {
        int len = _length;
        for (auto arg : *args)
        {
            len += arg->_length;
        }

        Reference<Array<T>> ref = Array<T>::createArray(len);
        typename TemplateType<T>::VarType* refData = ref->_data;
        for (int i = 0;i < _length;i++)
        {
            *refData = _data[i];
            refData++;
        }
        for (auto arg : *args)
        {
            for (int i = 0;i < arg->_length;i++)
            {
                *refData = arg->_data[i];
                refData++;
            }
        }
        return ref;
    }
}

