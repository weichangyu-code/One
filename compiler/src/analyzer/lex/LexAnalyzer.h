#pragma once
#include "LexElement.h"

/*
词法分析几种内容：
. 符号，例如{}[]()；
. 数值。以数字开头；
. 标识符；
. 空格；
. 特殊字符，例如：换行；
. 注释，支持#、C++注释；
. 关键字；
. 字符串；
*/

class LexAnalyzer
{
public:
	LexAnalyzer();

public:
	list<LexElement> analysis(const string& str);

protected:
	void finishAnalyBuf(int off_pos = 0);
	LexElement createElement(int type, int off_pos = 0);

public:
	list<LexElement> elements;
	enum
	{
		ANALY_NONE,
		ANALY_IDENTIFIER,
		ANALY_STRING,
		ANALY_CHAR,
		ANALY_NUMBER,
		ANALY_SYMBOL,
		ANALY_NOTE,
		ANALY_LINE_NOTE,
		ANALY_LINE_END,
	} analyState = ANALY_NONE;
	TextCoor analyPos;
	LexElement analyBuf;
};
