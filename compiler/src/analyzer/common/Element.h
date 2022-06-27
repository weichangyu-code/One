#pragma once
#include "Common.h"

class Element
{
public:
	enum TYPE
	{
		EMPTY,		//默认空
		SYMBOL,		//符号。value为符号的asill码
		STRING,		//字符串，用双引号包含的
		CHAR,		//字符
		KEYWORD,	//关键字。value为关键字索引
		NUMBER,		//数值
		IDENTIFIER, //标识符
		RULE,		//规则
		NOTE,		//注释
		LINEEND,	//行尾
		FILEEND
	};

public:
    Element() = default;
    Element(int type, int value = 0)
    {
        this->type = type;
        this->value = value;
    }

    void clear()
    {
        this->type = EMPTY;
        this->value = 0;
    }

	bool empty()
	{
		return this->type == EMPTY;
	}

public:
	int compare(const Element& r) const
	{
		return uv - r.uv;
	}

	bool operator < (const Element& r) const
	{
		return compare(r) < 0;
	}
	bool operator == (const Element& r) const
	{
		return compare(r) == 0;
	}

public:
	union
	{
		int uv = 0;
		struct
		{
			short type;
			short value;
		};
	};
	
};

