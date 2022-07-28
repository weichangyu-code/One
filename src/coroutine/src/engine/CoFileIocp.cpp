#include "CoFileIocp.h"
#include "Engine.h"
#include "CoEvent.h"

namespace OneCoroutine
{
    CoFile::CoFile()
        :_file(Engine::getCurEngine()->iocp)
    {

    }
    
    CoFile::~CoFile()
    {
        close();
    }

    int  CoFile::open(const char* path, int openFlag)
    {
        //加协程锁，不会知道整个线程堵塞。如果在_file内部加真锁，会堵塞
        CoMutexGuard g(_mtx);

        int ret = 0;
        Engine::getCurEngine()->executeOnPool([this, path, &ret, openFlag]() {
            ret = _file.open(path, openFlag);
        });
        return ret;
    }

    void CoFile::close()
    {
        CoMutexGuard g(_mtx);
        if (_file.isOpen() == false)
        {
            return;
        }
        Engine::getCurEngine()->executeOnPool([this]() {
            _file.close();
        });
    }
    int  CoFile::write(const char* data, unsigned int len)
    {
        CoMutexGuard g(_mtx);

        int ret = ERR_SUCCESS;
        CoEvent event;
        Engine::getCurEngine()->executeOnMain([=, &ret, &event]() {
            OperateOverlapped* oo = nullptr;
            _file.write(data, len, &oo, [&ret, &event](OperateOverlapped* oo) {
                if (oo->error == 0)
                {
                    ret = oo->trans;
                }
                else
                {
                    ret = oo->error;
                }
                event.signal();
            });
        });
        event.wait();
        return ret;
    }
    int  CoFile::read(char* data, unsigned int len)
    {
        CoMutexGuard g(_mtx);

        int ret = ERR_SUCCESS;
        CoEvent event;
        Engine::getCurEngine()->executeOnMain([=, &ret, &event]() {
            OperateOverlapped* oo = nullptr;
            _file.read(data, len, &oo, [&ret, &event](OperateOverlapped* oo) {
                if (oo->error == 0)
                {
                    ret = oo->trans;
                }
                else
                {
                    ret = oo->error;
                }
                event.signal();
            });
        });
        event.wait();
        return ret;
    }

}
