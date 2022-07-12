#pragma once


enum INSTRUCT
{
    NOP,                       //空指令
    INDEX,
    LINC,
    LDEC,
    RINC,
    RDEC,
    POS,
    NEG,
    NOT,
    BITNOT,
    MUL,
    DIV,
    REC,
    ADD,
    SUB,
    RBITMOV,
    RBITMOV2,
    LBITMOV,
    GT,
    GTE,
    LT,
    LTE,
    EQ,
    NEQ,
    EQ_DEEP,
    NEQ_DEEP,
    BITAND,
    BITXOR,
    BITOR,
    AND,						//右操作符可能为null
    OR,							//右操作符可能为null
    COND,                       // ? :
    ASSIGN,						//按照语法书写顺序，第二个操作符赋值给第一个
    DEEP_ASSIGN,

    MUL_ASSIGN,
    DIV_ASSIGN,
    REC_ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    RBITMOV_ASSIGN,
    RBITMOV2_ASSIGN,
    LBITMOV_ASSIGN,
    BITAND_ASSIGN,
    BITXOR_ASSIGN,
    BITOR_ASSIGN,
    AND_ASSIGN,
    OR_ASSIGN,

    OP_END,                     //操作符边界
        
    RETURN,
    BREAK,
    CONTINUE,

    NEW,                        //新建对象
    NEW_ARRAY,
    CALL,                       //函数调用，没有类前缀
    CALL_FIXED,                 //指定类函数调用，不会触发虚函数调用
    TYPE,                       //类型转换

    VARDEF,                     //变量定义
    BLOCK,                      //定义BLOCK
    IF,
    ELSE_IF,
    ELSE,

    // MUL_ASSIGN,
    // DIV_ASSIGN,
    // REC_ASSIGN,
    // ADD_ASSIGN,
    // SUB_ASSIGN,
    // RBITMOV_ASSIGN,
    // RBITMOV2_ASSIGN,
    // LBITMOV_ASSIGN,
    // BITAND_ASSIGN,
    // BITXOR_ASSIGN,
    // BITOR_ASSIGN,
};
