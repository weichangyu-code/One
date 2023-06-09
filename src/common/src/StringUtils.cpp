#include "StringUtils.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

namespace OneCommon
{
	bool StringUtils::isspace(char c)
	{
		return ::isspace((unsigned char)c) != 0;
	}
		
	size_t StringUtils::strlen(const char* str)
	{
		if (str == nullptr)
		{
			return 0;
		}
		return ::strlen(str);
	}
	
	string StringUtils::trim(const string& str)
	{
		const char* str2 = str.c_str();
		size_t s = 0;
		size_t e = str.size();

		while (s < e)
		{
			if (isspace(str2[s]) == false)
			{
				break;
			}
			s++;
		}

		while (s < e)
		{
			if (isspace(str2[e - 1]) == false)
			{
				break;
			}
			e--;
		}

		return str.substr(s, e - s);
	}

	string StringUtils::itoa(int v)
	{
		char buf[16];
		itoa(v, buf);
		return buf;
	}
		
	void StringUtils::itoa(int v, char* buf)
	{
	#ifdef _WIN32
		::itoa(v, buf, 10);
	#else
		sprintf(buf, "%d", v);
	#endif
	}

	string StringUtils::ltoa(long long v)
	{
		char buf[32];
	#ifdef _WIN32
		sprintf(buf, "%I64d", v);
	#else
		sprintf(buf, "%lld", v);
	#endif
		return buf;
	}
		
	string StringUtils::ftoa(float f)
	{
		char buf[32];
		sprintf(buf, "%f", f);
		return buf;
	}
		
	string StringUtils::dtoa(double d)
	{
		char buf[32];
		sprintf(buf, "%lf", d);
		return buf;
	}

	int StringUtils::atoi(const string& str)
	{
		return ::atoi(str.c_str());
	}
		
	int StringUtils::atoi(const char* str)
	{
		return ::atoi(str);
	}

	string StringUtils::replace(const string& str, const char* oldStr, const char* newStr)
	{
		size_t oldLen = strlen(oldStr);
		size_t newLen = strlen(newStr);

		string result;
		result.reserve(str.size() + oldLen);

		const char* str2 = str.c_str();
		string::size_type start = 0;
		string::size_type find = 0;
		while ((find = str.find(oldStr, start)) != str.npos)
		{
			result.append(str2 + start, find - start);
			result.append(newStr, newLen);
			start = find + oldLen;
		}

		result.append(str2 + start, str.size() - start);
		return result;
	}

	// 无法处理\r
	// list<string> StringUtils::getLines(const string& str)
	// {
	// 	istringstream stream(str);

	// 	list<string> lines;
	// 	string line;
	// 	while (std::getline(stream, line))
	// 	{
	// 		if (line.empty() == false)
	// 		{
	// 			lines.push_back(std::move(line));
	// 		}
	// 	}

	// 	return lines;
	// }

	list<string> StringUtils::getLines(const string& str, bool ignoreBlank)
	{
		const char* str2 = str.c_str();
		size_t size = str.size();

		list<string> lines;
		int start = 0;
		for (int i = 0;i < (int)size;i++)
		{
			char c = str2[i];

			if ((c == '\r') || (c == '\n'))
			{
				if (ignoreBlank == false || start < i)
				{
					lines.push_back(str.substr(start, i - start));
				}

				if ((c == '\r') && (((i + 1) < (int)size) && (str2[i + 1] == '\n')))
				{
					i++;
				}
				
				start = i + 1;
			}
		}
		if (ignoreBlank == false || start < (int)size)
		{
			lines.push_back(str.substr(start, size - start));
		}

		return lines;
	}

	list<string> StringUtils::splitString(const string& str, const char* sep)
	{
		list<string> strs;

		size_t sepLen = strlen(sep);
		string::size_type start = 0;
		string::size_type find = 0;
		while ((find = str.find(sep, start)) != str.npos)
		{
			strs.push_back(str.substr(start, find - start));
			start = find + sepLen;
		}
		strs.push_back(str.substr(start));
		return strs;
	}

	string StringUtils::toLower(const string& str)
	{
		string ret = str;
		char* str2 = (char*)ret.data();
		char* end = str2 + ret.size();
		while (str2 < end)
		{
			*str2 = toLower(*str2);
			str2++;
		}
		return ret;
	}

	string StringUtils::toUpper(const string& str)
	{
		string ret = str;
		char* str2 = (char*)ret.data();
		char* end = str2 + ret.size();
		while (str2 < end)
		{
			*str2 = toUpper(*str2);
			str2++;
		}
		return ret;
	}

	string StringUtils::toFirstUpper(const string& str)
	{
		string ret = str;
		if (ret.empty() == false)
		{
			ret[0] = toUpper(ret[0]);
		}
		return ret;
	}
		
	char StringUtils::toUpper(char c)
	{
		return ::toupper((unsigned char)c);
	}
	
	char StringUtils::toLower(char c)
	{
		return ::tolower((unsigned char)c);
	}

	void StringUtils::strncpy_z(char* dest, const char* src, unsigned int dest_buf_len)
	{
		if (dest_buf_len <= 0)
		{
			return;
		}
		strncpy(dest, src, dest_buf_len - 1);
		dest[dest_buf_len - 1] = '\0';
	}

	void StringUtils::strncat_z(char* dest, const char* src, unsigned int dest_buf_len)
	{
		unsigned int dest_len = (unsigned int)strlen(dest);
		if (dest_buf_len <= dest_len)
		{
			//dest_buf_len==dest_len时，返回后的dest可能不以0结尾。但不考虑输入错误的情况下，输出正确。
			return;
		}
		strncpy(dest + dest_len, src, dest_buf_len - dest_len - 1);
		dest[dest_buf_len - 1] = '\0';
	}
		
	string StringUtils::format(const char* fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);
		int size = vsnprintf(nullptr, 0, fmt, ap);
		va_end(ap);
		if (size == -1)
		{
			return "";
		}

		string str;
		str.resize(size);
		
		va_list ap2;
		va_start(ap2, fmt);
		vsprintf((char*)str.data(), fmt, ap2);
		va_end(ap2);

		return str;
	}
}
