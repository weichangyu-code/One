#pragma once
#include "Common.h"

namespace OneCommon
{
	class FileUtils
	{
	public:
		static list<string> listSub(const string& folder);
		static bool isDir(const string& path);
		static bool createDir(const string& path);
		static bool remove(const string& path);
		static bool exist(const string& path);

		static string readFile(const string& path);

		//目录的/后缀
		static string addDirSuffix(const string& path);
		static string delDirSuffix(const string& path);
		static string appendFileName(const string& path, const string& name);
		static string getDir(const string& path);							//返回的值，不带后缀
		static string getFileName(const string& path);
		static string getFileTitle(const string& path);
		static string getFileExt(const string& path);

		static bool isAbsolutePath(const string& path);				//判断是不是绝对目录
		static bool isRelativePath(const string& path);	
	};
}