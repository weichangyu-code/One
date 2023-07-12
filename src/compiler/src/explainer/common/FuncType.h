#pragma once

enum FUNC_TYPE
{
    FUNC_NORMAL,
    FUNC_INIT,       //变量初始化，函数名为空
    FUNC_STATIC_INIT,//静态初始化
    FUNC_CONSTRUCT,  //构造
    FUNC_DESTRUCT,   //析构
};
