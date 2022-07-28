#include "OneFile.h"
#include "engine/CoFile.h"

namespace One
{
    File::File()
    {
        _file = new OneCoroutine::CoFile();
    }
        
    File::~File()
    {
        delete _file;
    }

    int  File::open(String* path, int openFlag)
    {
        return _file->open(path->str(), openFlag);
    }
        
    void File::close()
    {
        _file->close();
    }

    int  File::write(Buffer* buffer)
    {
        int ret = _file->write((const char*)buffer->getBuf() + buffer->_readPos, buffer->_writePos);
        if (ret > 0)
        {
            buffer->_writePos = ret;
        }
        return ret;
    }
    int  File::read(Buffer* buffer)
    {
        int ret = _file->read((char*)buffer->getBuf() + buffer->_writePos, buffer->getLength() - buffer->_writePos);
        if (ret > 0)
        {
            buffer->_writePos += ret;
        }
        return ret;
    }

} // namespace One
