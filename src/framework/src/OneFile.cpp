#include "OneFile.h"
#include "engine/CoFile.h"

namespace One
{
    File::File()
    {
        _file = new OneCoroutine::CoFile();
    }
        
    File::File(String* path, OneInt openFlag, OneInt mode)
        :File()
    {
        int err = open(path, openFlag, mode);
        setLastError(err);
    }
        
    File::~File()
    {
        delete _file;
    }

    OneInt File::open(String* path, OneInt openFlag, OneInt mode)
    {
        return _file->open((const char*)path->str(), openFlag, mode);
    }
        
    void File::close()
    {
        _file->close();
    }

    OneInt File::write(Buffer* buffer)
    {
        int ret = _file->write((const char*)buffer->getData(), (unsigned int)buffer->getDataLength());
        if (ret > 0)
        {
            buffer->addReadPos(ret);
        }
        return ret;
    }
    OneInt File::read(Buffer* buffer)
    {
        int ret = _file->read((char*)buffer->getLeftBuf(), (unsigned int)buffer->getLeftCapacity());
        if (ret > 0)
        {
            buffer->addWritePos(ret);
        }
        return ret;
    }
    OneBool File::isOpen()
    {
        return _file->isOpen();
    }

} // namespace One
