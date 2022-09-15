
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
        return Reference<T2>(r.getObject(), r.isInner(), true);
    }
}
