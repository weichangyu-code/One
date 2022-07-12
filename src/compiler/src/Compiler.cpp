

#include "Version.h"
#include "./project/Project.h"

int main(int argc, char* argv[])
{
    //解析参数，获取工程目录
    string projectFolder;
    for (int i = 1;i < argc;i++)
    {
        char* param = argv[i];
        if (param[0] == '-')
        {
            //参数
            if (param[1] == 0 || param[2] != 0)
            {
                printf("unkown param %s\n", param);
                return -1;
            }

            switch (param[1])
            {
            case 'v':
                printf("version %s. build %s %s\n", version, __DATE__, __TIME__);
                return 0;
            case 'h':
                printf("one [-v] [-h] [projectFolder]\n");
                return 0;
            default:
                printf("unkown param %s\n", param);
                return -1;
            }
        }

        if (projectFolder.empty())
        {
            projectFolder = param;
        }
        else
        {
            printf("unkown param %s\n", param);
            return -1;
        }
    }
    if (projectFolder.empty())
    {
        projectFolder = "./";
    }

    //加载工程
    Project project;
    Result result = project.build(projectFolder);
    if (result.isError())
    {
        printf("build failed. %s\n", result.errMsg.c_str());
        return -1;
    }

    printf("build success.\n");
    return 0;
}
