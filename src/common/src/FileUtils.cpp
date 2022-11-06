#include "FileUtils.h"
#include <time.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>


#define PATH_SEPARATE				'/'
#define PATH_SEPARATE2				'\\'
#define IS_PATH_SEPARATE(c)			(((c) == PATH_SEPARATE) || ((c) == PATH_SEPARATE2))
#define FILE_EXT_PREFIX				'.'

namespace OneCommon
{
    bool FileUtils::exist(const string& path)
    {
        struct stat fileStat;
        if (stat(path.c_str(), &fileStat) == 0)
        {
            return true;
        }
        return false;
    }

    string FileUtils::readFile(const string& path)
    {
        ifstream file(path);
        ostringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }

    string FileUtils::addDirSuffix(const string& path)
    {
        if (path.length() == 0)
        {
            return path + PATH_SEPARATE;
        }
        else if (IS_PATH_SEPARATE(path[path.length() - 1]) == false)
        {
            return path + PATH_SEPARATE;
        }
        return path;
    }

    string FileUtils::delDirSuffix(const string& path)
    {
        if (path.empty() == false && IS_PATH_SEPARATE(path.back()))
        {
            return path.substr(0, path.length() - 1);
        }
        return path;
    }
        
    string FileUtils::appendFileName(const string& path, const string& name)
    {
        if (path.empty())
        {
            return name;
        }
        else
        {
            return addDirSuffix(path) + name;
        }
    }

    string FileUtils::getDir(const string& path)
    {
        size_t find = path.find_last_of(PATH_SEPARATE);
        if (find == path.npos)
        {
            find = path.find_last_of(PATH_SEPARATE2);
            if (find == path.npos)
            {
                return "";
            }
        }
        return path.substr(0, find);
    }

    string FileUtils::getFileName(const string& path)
    {
        size_t find = path.find_last_of(PATH_SEPARATE);
        if (find == path.npos)
        {
            find = path.find_last_of(PATH_SEPARATE2);
            if (find == path.npos)
            {
                return path;
            }
        }
        return path.substr(find + 1);
    }

    string FileUtils::getFileTitle(const string& path)
    {
        string fileName = getFileName(path);
        size_t find = fileName.find_last_of(FILE_EXT_PREFIX);
        if (find == fileName.npos)
        {
            return fileName;
        }
        return fileName.substr(0, find);
    }

    string FileUtils::getFileExt(const string& path)
    {
        string fileName = getFileName(path);
        size_t find = fileName.find_last_of(FILE_EXT_PREFIX);
        if (find == fileName.npos)
        {
            return "";
        }
        return fileName.substr(find + 1);
    }
	
    bool FileUtils::isAbsolutePath(const string& path)
    {
        if (path.empty())
        {
            return false;
        }
        if (IS_PATH_SEPARATE(path.front()))
        {
            return true;
        }
        if (path.size() >= 2)
        {
            if (path[1] == ':')
            {
                //windows的路径
                return true;
            }
        }
        return false;
    }
	
    bool FileUtils::isRelativePath(const string& path)
    {
        return isAbsolutePath(path) == false;
    }
}