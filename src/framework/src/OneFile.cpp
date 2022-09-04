#include "OneFile.h"
#include "engine/CoFile.h"

namespace One
{
    File::File()
    {
        _file = new OneCoroutine::CoFile();
    }
        
    File::File(String* path, int openFlag, int mode)
        :File()
    {
        int err = open(path, openFlag, mode);
        setLastError(err);
    }
        
    File::~File()
    {
        delete _file;
    }

    int  File::open(String* path, int openFlag, int mode)
    {
        return _file->open(path->str(), openFlag, mode);
    }
        
    void File::close()
    {
        _file->close();
    }

    int  File::write(Buffer* buffer)
    {
        int ret = _file->write(buffer->getData(), buffer->getDataLength());
        if (ret > 0)
        {
            buffer->addReadPos(ret);
        }
        return ret;
    }
    int  File::read(Buffer* buffer)
    {
        int ret = _file->read(buffer->getLeftBuf(), buffer->getLeftCapacity());
        if (ret > 0)
        {
            buffer->addWritePos(ret);
        }
        return ret;
    }
    bool File::isOpen()
    {
        return _file->isOpen();
    }

} // namespace One
