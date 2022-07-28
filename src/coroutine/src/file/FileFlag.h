#pragma once

namespace OneCoroutine
{
    enum FILE_OPEN_FLAG
    {
        READ_ONLY = 0x1,
        WRITE_ONLY = 0x2,
        READ_WRITE = 0x3,

        CREATE = 0x10,          //不存在则创建
        TRUNCATE = 0x20,        //清空文件内容
    };

}
