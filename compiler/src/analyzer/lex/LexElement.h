#pragma once
#include "../common/Element.h"

class TextCoor
{
public:
	void clear()
	{
		pos = line = col = 0;
	}

	string toString();

public:
	int pos = 0;			//字节偏移
	int line = 0;			//多少行
	int col = 0;			//多少列
};

class LexElement : public Element
{
public:
	LexElement() = default;
	LexElement(int type, int value = 0, const string& remark = "")
		: Element(type, value)
	{
		if (remark.empty())
		{
			if (type == LINEEND)
			{
				this->remark = "LINEEND";
			}
			else if (type == FILEEND)
			{
				this->remark = "FILEEND";
			}
		}
		else
		{
			this->remark = remark;
		}
	}
	
	void clear()
	{
		Element::clear();
		start.clear();
		end.clear();
		remark.clear();
		ptr = nullptr;
	}

	string toString();
	string toSimpleString();

public:
	string remark;
	TextCoor start, end;

	void*  ptr = nullptr;
};
