

namespace One
{
    template<typename T>
    void Pointer<T>::acquire()
    {
        if (std::is_base_of<Object, T>::value)
        {
            Object* obj = (Object*)(void*)getObject();
            obj->__acquire__(isInner());
        }
        else
        {
            //规避多重继承的歧义问题
            Interface* i = (Interface*)(void*)getObject();
            i->__acquireObj__(isInner());
        }
    }
    
    template<typename T>
    void Pointer<T>::release()
    {
        if (std::is_base_of<Object, T>::value)
        {
            Object* obj = (Object*)(void*)getObject();
            obj->__release__(isInner());
        }
        else
        {
            //规避多重继承的歧义问题
            Interface* i = (Interface*)(void*)getObject();
            i->__releaseObj__(isInner());
        }
    }

    template<typename T1, typename T2>
    Pointer<T2> convertPointer(Pointer<T1> r)
    {
        if (std::is_base_of<Object, T2>::value)
        {
            //转对象，ptr值不会有变化，强转
            return (Pointer<T2>&)r;
        }
        else
        {
            return Pointer<T2>(r.getObject(), r.isInner());
        }
    }
}    
