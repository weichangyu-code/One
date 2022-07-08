#include "LexAnalyzer.h"
#include <ctype.h>

LexAnalyzer::LexAnalyzer()
{
	
}

list<LexElement> LexAnalyzer::analysis(const string& str)
{
	const char* code = str.c_str();
	size_t size = str.size();

	for (int i = 0;i < size;)
	{
		char c = code[i];
		char nc = code[i + 1];
		int  use = 0;
		bool isTab = false;

		if (analyState == ANALY_STRING)
		{
			if ((c == '\\') && (nc == '\\'))
			{
				analyBuf.remark += c;
				analyBuf.remark += nc;
				use = 2;
			}
			else if ((c == '\\') && (nc == '\"'))
			{
				analyBuf.remark += c;
				analyBuf.remark += nc;
				use = 2;
			}
			else if (c == '\"')
			{
				//字符串结束
				finishAnalyBuf();
				use = 1;
			}
			else if ((c == '\r') || (c == '\n'))
			{
				finishAnalyBuf();
			}
			else
			{
				analyBuf.remark += c;
				use = 1;
			}
		}
		else if (analyState == ANALY_CHAR)
		{
			if ((c == '\\') && (nc == '\\'))
			{
				analyBuf.remark += c;
				analyBuf.remark += nc;
				use = 2;
			}
			else if ((c == '\\') && (nc == '\''))
			{
				analyBuf.remark += c;
				analyBuf.remark += nc;
				use = 2;
			}
			else if (c == '\'')
			{
				//字符串结束
				finishAnalyBuf();
				use = 1;
			}
			else if ((c == '\r') || (c == '\n'))
			{
				finishAnalyBuf();
			}
			else
			{
				analyBuf.remark += c;
				use = 1;
			}
		}
		else if (analyState == ANALY_LINE_NOTE)
		{
			if ((c == '\r') || (c == '\n'))
			{
				finishAnalyBuf();
			}
			else
			{
				analyBuf.remark += c;
				use = 1;
			}
		}
		else if (analyState == ANALY_NOTE)
		{
			if ((c == '*') && (nc == '/'))
			{
				use = 2;
				finishAnalyBuf();
			}
			else
			{
				analyBuf.remark += c;
				use = 1;
			}
		}
		else if (analyState == ANALY_IDENTIFIER)
		{
			if (c == '_')
			{
				analyBuf.remark += c;
				use = 1;
			}
			else if (isspace(c) || ispunct(c))
			{
				finishAnalyBuf();
			}
			else
			{
				analyBuf.remark += c;
				use = 1;
			}
		}
		else if (analyState == ANALY_NUMBER)
		{
			if (c == 'e' || c == 'E')
			{
				//指数表示，e后面会跟-、+、数字
				analyBuf.remark += c;
				analyBuf.remark += nc;
				use = 2;
			}
			else if (isspace(c) || (ispunct(c) && (c != '.')))
			{
				finishAnalyBuf();
			}
			else
			{
				analyBuf.remark += c;
				use = 1;
			}
		}
		
		if (use == 0)
		{
			//
			if ((c == '\r') || (c == '\n'))
			{
				if ((elements.empty() == false) && (elements.back().type != Element::LINEEND))
				{
					analyBuf = createElement(Element::LINEEND);
					analyState = ANALY_LINE_END;
					finishAnalyBuf(0);
				}
				use = 1;
			}
			else if ((c == '/') && (nc == '/'))
			{
				analyBuf = createElement(Element::NOTE);
				analyState = ANALY_LINE_NOTE;
				use = 2;
			}
			else if ((c == '/') && (nc == '*'))
			{
				analyBuf = createElement(Element::NOTE);
				analyState = ANALY_NOTE;
				use = 2;
			}
			else if (c == '\'')
			{
				analyBuf = createElement(Element::CHAR);
				analyState = ANALY_CHAR;
				use = 1;
			}
			else if (c == '\"')
			{
				analyBuf = createElement(Element::STRING);
				analyState = ANALY_STRING;
				use = 1;
			}
			else if (isalpha(c) || c == '_')
			{
				analyBuf = createElement(Element::IDENTIFIER);
				analyBuf.remark += c;
				analyState = ANALY_IDENTIFIER;
				use = 1;
			}
			else if (ispunct(c))
			{
				analyBuf = createElement(Element::SYMBOL);
				analyBuf.value = c;
				analyBuf.remark += c;
				analyState = ANALY_SYMBOL;
				finishAnalyBuf(0);
				use = 1;
			}
			else if (isdigit(c))
			{
				analyBuf = createElement(Element::NUMBER);
				analyBuf.remark += c;
				analyState = ANALY_NUMBER;
				use = 1;
			}
			else if (c == '\t')
			{
				use = 1;
				isTab = true;
			}
			else
			{
				use = 1;
			}
		}

		if ((c == '\r') || (c == '\n'))
		{
			analyPos.line++;
			analyPos.col = 0;
		}
		else
		{
			analyPos.col += isTab ? 4 : use;
		}
		analyPos.pos += isTab ? 4 : use;
		i += use;
	}

	finishAnalyBuf(0);
	analyPos.clear();

	elements.push_back(LexElement(Element::FILEEND));

	return std::move(elements);
}

void LexAnalyzer::finishAnalyBuf(int off_pos)
{
	if (analyState == ANALY_NONE)
	{
		return;
	}
	analyBuf.end = analyPos;
	analyBuf.end.pos += off_pos;
	analyBuf.end.col += off_pos;

	elements.push_back(std::move(analyBuf));
	analyBuf.clear();
	analyState = ANALY_NONE;
}

LexElement LexAnalyzer::createElement(int type, int off_pos)
{
	LexElement ele(type);
	ele.start = ele.end = analyPos;
	ele.end.pos += off_pos;
	ele.end.col += off_pos;
	return ele;
}
