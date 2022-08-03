
#ifndef _WIN32

#include "Common.h"
#include "File.h"
#include "../FileFlag.h"
#include "../../common/Error.h"
#include <fcntl.h>
#include <unistd.h>

namespace OneCoroutine
{
    File::File()
    {
    }

    File::~File()
    {

    }

    int File::open(const char* path, int openFlag, int mode)
    {
        close();

        int flags = 0;
        switch (openFlag & FILE_OPEN_FLAG::READ_WRITE)
        {
        case FILE_OPEN_FLAG::READ_WRITE:
            flags |= O_WRONLY;
            break;
        case FILE_OPEN_FLAG::READ_ONLY:
            flags |= O_RDONLY;
            break;
        case FILE_OPEN_FLAG::WRITE_ONLY:
            flags |= O_RDWR;
            break;
        default:
            flags |= O_RDWR;
            break;
        }

        switch (openFlag & (FILE_OPEN_FLAG::CREATE | FILE_OPEN_FLAG::TRUNCATE))
        {
        case FILE_OPEN_FLAG::CREATE | FILE_OPEN_FLAG::TRUNCATE:
            flags |= O_CREAT|O_TRUNC;
            break;
        case FILE_OPEN_FLAG::CREATE:
            flags |= O_CREAT;
            break;
        case FILE_OPEN_FLAG::TRUNCATE:
            flags |= O_TRUNC;
            break;
        default:
            flags |= O_CREAT;
            break;
        }

        _fd = ::open(path, flags, mode);
        if (_fd == -1)
        {
            switch (errno)
            {
            case EACCES:
                return ERR_FILE_ACCESS_DENIED;
                break;
            default:
                return ERR_ERROR;
                break;
            }
        }

        return ERR_SUCCESS;
    }
    
    void File::close()
    {
        if (_fd != -1)
        {
            ::close(_fd);
            _fd = -1;
        }
    }
        
    bool File::isOpen()
    {
        return _fd != -1;
    }
        
    int File::write(const char* data, unsigned int len)
    {
        if (_fd == -1)
        {
            return ERR_ERROR;
        }

        int ret = ::write(_fd, data, len);
        if (ret == -1)
        {
            return ERR_ERROR;
        }
        return ret;
    }

    int File::read(char* data, unsigned int len)
    {
        if (_fd == -1)
        {
            return ERR_ERROR;
        }

        int ret = ::read(_fd, data, len);
        if (ret == -1)
        {
            return ERR_ERROR;
        }
        return ret;
    }
}

#endif
