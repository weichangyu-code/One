#include "FolderUtils.h"
#include <sys/stat.h>
#include "FileUtils.h"

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

namespace OneCommon
{
    void FolderUtils::findEach(const string& path, const function<bool(const string& name, bool isDir)>& func)
    {
#ifdef _WIN32
        _finddata_t fileInfo;
        intptr_t handle = _findfirst(FileUtils::appendFileName(path, "*").c_str(), &fileInfo);
        if (handle == -1)
        {
            return;
        }

        do
        {
            if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
            {
                if (func(fileInfo.name, fileInfo.attrib == _A_SUBDIR) == false)
                {
                    //中断
                    break;
                }
            }
        } while (_findnext(handle, &fileInfo) != -1);
        _findclose(handle);
#else
        DIR* dir = opendir(path.c_str());
        if (dir == nullptr)
        {
            return;
        }
        dirent* d = readdir(dir);
        while (d)
        {
            if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
            {
                if (func(d->d_name, d->d_type == DT_DIR) == false)
                {
                    //中断
                    break;
                }
            }
            d = readdir(dir);
        }
        closedir(dir);
#endif
    }

    bool FolderUtils::isDir(const string& path)
    {
        struct stat fileStat;
        if (::stat(path.c_str(), &fileStat) == 0)
        {
            return (fileStat.st_mode & S_IFDIR) != 0;
        }
        return false;
    }

    bool FolderUtils::createDir(const string& path)
    {
#ifdef _WIN32
        ::mkdir(path.c_str());
#else
        ::mkdir(path.c_str(), 0777);
#endif
        return true;
    }
		
    bool FolderUtils::removeDir(const string& path)
    {
        findEach(path, [path](const string& name, bool isDir) {
            string path2 = FileUtils::appendFileName(path, name);
            if (isDir)
            {
                removeDir(path2);
            }
            else
            {
                ::remove(path2.c_str());
            }
            return true;
        });
        return ::rmdir(path.c_str()) != -1;
    }
    
} // namespace OneCommon