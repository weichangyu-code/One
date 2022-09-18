
namespace One
{
    template<typename T1, typename T2>
    Reference<T2>& convertObjectReference(const Reference<T1>& r)
    {
        return (Reference<T2>&)r;
    }

    template<typename T1, typename T2>
    Reference<T2> convertInterfaceReference(const Reference<T1>& r)
    {
        if (r.isNull())
        {
            //避免0值变成非0值
            return Reference<T2>();
        }
        else
        {
            return Reference<T2>(r.getObject(), r.isInner(), true);
        }
    }

    template<typename T1, typename T2>
    Reference<T2>& convertReferenceForce(const Reference<T1>& r)
    {
        return (Reference<T2>&)r;
    }

}
