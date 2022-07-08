#pragma once
#include "Common.h"

namespace OneCommon
{
    class CollectionUtils
    {
    public:
        template<class T>
        static int compare(const T& t1, const T& t2
            , int (*compareValue)(typename T::const_reference v1, typename T::const_reference v2))
        {
            if (t1.size() != t2.size())
            {
                return (int)t1.size() - (int)t2.size();
            }

            typename T::const_iterator iter1 = t1.begin();
            typename T::const_iterator iter2 = t2.begin();
            while (iter1 != t1.end())
            {
                int cmp = (*compareValue)(*iter1, *iter2);
                if (cmp != 0)
                {
                    return cmp;
                }
                ++iter1;
                ++iter2;
            }
            return 0;
        }
    };
}
