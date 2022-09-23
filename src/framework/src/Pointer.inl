

namespace One
{
    template<typename T>
    void Pointer<T>::acquire()
    {
        if (std::is_base_of<Object, T>::value)
        {
            Object* obj = (Object*)(void*)getObject();
            obj->__acquire__(isOwner());
        }
        else
        {
            //规避多重继承的歧义问题
            Interface* i = (Interface*)(void*)getObject();
            i->__acquireObj__(isOwner());
        }
    }
    
    template<typename T>
    void Pointer<T>::release()
    {
        if (std::is_base_of<Object, T>::value)
        {
            Object* obj = (Object*)(void*)getObject();
            obj->__release__(isOwner());
        }
        else
        {
            //规避多重继承的歧义问题
            Interface* i = (Interface*)(void*)getObject();
            i->__releaseObj__(isOwner());
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
            if (r.isNull())
            {
                //子类变成基类，如果强转，会变成非0值
                return Pointer<T2>();
            }
            else
            {
                return Pointer<T2>(r.getObject(), r.isOwner());
            }
        }
    }

    template<typename T1, typename T2>
    Pointer<T2> convertPointerForce(Pointer<T1> r)
    {
        return (Pointer<T2>&)r;
    }
}    
