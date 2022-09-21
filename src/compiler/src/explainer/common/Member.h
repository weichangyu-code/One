#pragma once

enum MEMBER_FILTER_TYPE
{
    MFT_ALL = 0x3,
    MFT_ONLY_STATIC = 0x1,        //静态函数
    MFT_ONLY_NORMAL = 0x2,        //普通函数
};

inline bool filterMember(bool isStatic, int filterType)
{
    return (isStatic && (filterType & MFT_ONLY_STATIC)) || (isStatic == false && (filterType & MFT_ONLY_NORMAL));
}