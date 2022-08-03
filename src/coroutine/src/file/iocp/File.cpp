
#ifdef _WIN32

#include "File.h"
#include "../FileFlag.h"
#include "../../socket/iocp/Iocp.h"
#include "../../common/Error.h"

namespace OneCoroutine
{
    File::File(Iocp* iocp)
    {
        this->iocp = iocp;
    }

    File::~File()
    {

    }

    int File::open(const char* path, int openFlag, int mode)
    {
        close();

        DWORD dwDesiredAccess = 0;
        DWORD dwShareMode = 0;
        DWORD dwCreationDisposition = 0;
        DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED;

        switch (openFlag & FILE_OPEN_FLAG::READ_WRITE)
        {
        case FILE_OPEN_FLAG::READ_WRITE:
            dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
            break;
        case FILE_OPEN_FLAG::READ_ONLY:
            dwDesiredAccess = GENERIC_READ;
            break;
        case FILE_OPEN_FLAG::WRITE_ONLY:
            dwDesiredAccess = GENERIC_WRITE;
            break;
        default:
            dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
            break;
        }

        switch (openFlag & (FILE_OPEN_FLAG::CREATE | FILE_OPEN_FLAG::TRUNCATE))
        {
        case FILE_OPEN_FLAG::CREATE | FILE_OPEN_FLAG::TRUNCATE:
            dwCreationDisposition = CREATE_ALWAYS;
            break;
        case FILE_OPEN_FLAG::CREATE:
            dwCreationDisposition = OPEN_ALWAYS;
            break;
        case FILE_OPEN_FLAG::TRUNCATE:
            dwCreationDisposition = TRUNCATE_EXISTING;
            break;
        default:
            dwCreationDisposition = OPEN_ALWAYS;
            break;
        }

        hFile = CreateFile(path, dwDesiredAccess, dwShareMode, nullptr, dwCreationDisposition, dwFlagsAndAttributes, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            switch (GetLastError())
            {
            case ERROR_FILE_NOT_FOUND:
                return ERR_FILE_NOT_FOUND;
                break;
            case ERROR_ACCESS_DENIED:
            case ERROR_SHARING_VIOLATION:
                return ERR_FILE_ACCESS_DENIED;
                break;
            default:
                return ERR_ERROR;
                break;
            }
        }

        iocp->registerEvent(hFile);

        return ERR_SUCCESS;
    }
    
    void File::close()
    {
        if (hFile != INVALID_HANDLE_VALUE)
        {
            iocp->unregisterEvent(hFile);

            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
        }
    }
        
    bool File::isOpen()
    {
        return hFile != INVALID_HANDLE_VALUE;
    }
        
    void File::write(const char* data, unsigned int len, OperateOverlapped** ooOut, const OOCompleteCB& cb)
    {
        OperateOverlapped* oo = iocp->mallocFromPool();
        oo->handle = hFile;
        oo->cbUser = cb;
        oo->cb = [this](OperateOverlapped* oo) {
            oo->cbUser(oo);
            iocp->freeToPool(oo);
        };
        *ooOut = oo;

        if (hFile == INVALID_HANDLE_VALUE)
        {
            //统一返回
            oo->error = ERR_ERROR;
            oo->cb(oo);
            return;
        }

        DWORD writed = 0;
        BOOL result = WriteFile(hFile, data, len, &writed, &oo->ol);
        if (result == FALSE)
        {
            int err = GetLastError();
            if (err != ERROR_IO_PENDING)
            {
                oo->error = ERR_ERROR;
                oo->cb(oo);
                return;
            }
        }
    }

    void File::read(char* data, unsigned int len, OperateOverlapped** ooOut, const OOCompleteCB& cb)
    {
        OperateOverlapped* oo = iocp->mallocFromPool();
        oo->handle = hFile;
        oo->cbUser = cb;
        oo->cb = [this](OperateOverlapped* oo) {
            oo->cbUser(oo);
            iocp->freeToPool(oo);
        };
        *ooOut = oo;

        if (hFile == INVALID_HANDLE_VALUE)
        {
            //统一返回
            oo->error = ERR_ERROR;
            oo->cb(oo);
            return;
        }

        DWORD readed = 0;
        BOOL result = ReadFile(hFile, data, len, &readed, &oo->ol);
        if (result == FALSE)
        {
            int err = GetLastError();
            if (err != ERROR_IO_PENDING)
            {
                oo->error = ERR_ERROR;
                oo->cb(oo);
                return;
            }
        }
    }
}

#endif
