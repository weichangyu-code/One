#pragma once
#include "Common.h"

namespace OneCommon
{
	class FolderUtils
	{
	public:
        static void findEach(const string& path, const function<bool(const string& name, bool isDir)>& func);
		
		static bool isDir(const string& path);
		static bool createDir(const string& path);
		static bool removeDir(const string& path);
	};
}