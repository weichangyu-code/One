#pragma once

enum
{
    VAR_LOCAL,          //局部变量
    VAR_PARAM,          //函数参数
    VAR_MEMBER,         //类成员
    VAR_THIS,           
    VAR_SUPER,
    VAR_ANONY_THIS,     //用在匿名类上外层的THIS
};