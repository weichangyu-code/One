#pragma once
#include "Common.h"

namespace OneCommon
{
	class StringUtils
	{
	public:
		static string trim(const string& str);

		// 注意：只支持10进制转换
		static string itoa(int v);
		static string ltoa(long long v);
		static string ftoa(float f);
		static string dtoa(double d);
		static int atoi(const char* str);
		static int atoi(const string& str);
		static void itoa(int v, char* buf);

		static string replace(const string& str, const char* oldStr, const char* newStr);
		static list<string> getLines(const string& str, bool ignoreBlank = true);
		static list<string> splitString(const string& str, const char* sep);

		static string toLower(const string& str);
		static string toUpper(const string& str);
		static string toFirstUpper(const string& str);
		static char toUpper(char c);

		static void strncpy_z(char* dest, const char* src, unsigned int dest_buf_len);
		static void strncat_z(char* dest, const char* src, unsigned int dest_buf_len);

		static string format(const char* fmt, ...);
	};
}

