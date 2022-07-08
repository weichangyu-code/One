#include "LexElement.h"
#include "StringUtils.h"
using namespace OneCommon;

string TextCoor::toString()
{
	char buf[128] = { 0 };
	sprintf(buf, "(ps=%d,line=%d,col=%d)", pos, line, col);
	return buf;
}

string LexElement::toString()
{
	string str;
	str += "[type=";
	str += StringUtils::itoa(type);
	str += ";value=";
	str += StringUtils::itoa(value);
	str += ";remark=";
	str += remark;
	str += ";start=";
	str += start.toString();
	str += ";end=";
	str += end.toString();
	str += "]";
	return str;
}

string LexElement::toSimpleString()
{
	string str;
	switch (type)
	{
	case STRING: str = "\"" + remark + "\"";break;
	case CHAR: str = "\'" + remark + "\'";break;
	default:
		if (remark.empty() == false)
		{
			str = remark;
		}
		else
		{
			str += "Element(";
			str += StringUtils::itoa(type);
			str += ")";
		}
		break;
	}
	return str;
}
