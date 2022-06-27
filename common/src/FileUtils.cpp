#include "FileUtils.h"
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <ftw.h>
#include <unistd.h>

#define PATH_SEPARATE				'/'
#define PATH_SEPARATE2				'\\'
#define IS_PATH_SEPARATE(c)			(((c) == PATH_SEPARATE) || ((c) == PATH_SEPARATE2))
#define FILE_EXT_PREFIX				'.'

namespace OneCommon
{
    list<string> FileUtils::listSub(const string& folder)
    {
        DIR* dir = opendir(folder.c_str());
        if (dir == nullptr)
        {
            return {};
        }
        list<string> subs;
        dirent* d = readdir(dir);
        while (d)
        {
            if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
            {
                subs.push_back(d->d_name);
            }
            d = readdir(dir);
        }
        closedir(dir);
        return subs;
    }

    bool FileUtils::isDir(const string& path)
    {
        struct stat fileStat;
        if (stat(path.c_str(), &fileStat) == 0)
        {
            return S_ISDIR(fileStat.st_mode);
        }
        return false;
    }
        
    bool FileUtils::createDir(const string& path)
    {
    #ifdef _WIN32
        mkdir(path.c_str());
    #else
        mkdir(path.c_str(), 0);
    #endif
        return true;
    }
        
    //这个太危险了，慎用
    bool FileUtils::remove(const string& path)
    {
        nftw(path.c_str(), [](const char *fpath, const struct stat *sb, int typeflag, struct FTW *FTWbuff) {
            if (typeflag == FTW_F)
            {
                ::remove(fpath);
            }
            else if (typeflag == FTW_D || typeflag == FTW_DP)
            {
                ::rmdir(fpath);
            }
            return 0;
        }, 50, FTW_DEPTH|FTW_PHYS);
        return true;
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
}