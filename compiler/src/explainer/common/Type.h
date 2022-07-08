#pragma once

enum DATA_TYPE
{
    DT_NONE,
    DT_NULL,               //null的类型，可以赋值给CLASS
    DT_VOID,
    DT_BOOL,
    DT_CHAR,               //有符号型
    DT_SHORT,
    DT_INT,
    DT_LONG,
    DT_FLOAT,
    DT_DOUBLE,
    DT_STRING,             //常量类型有效
    DT_CLASS,
};
