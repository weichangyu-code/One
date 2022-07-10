#include "Project.h"
#include "FileUtils.h"
#include "ProjectConfigLoader.h"
#include "StringUtils.h"

Project::Project()
    :cppGenerator(oneExplainer.getMetaContainer())
{

}

Result Project::build(const string& folder)
{
    projectFolder = folder;
    projectFolderByBuild = "../";           //工作目录在build的时候，当前的工程目录
    //加载工程配置文件
    string projectFile = FileUtils::appendFileName(projectFolder, "one.json");
    if (ProjectConfigLoader::load(projectFile, &projectConfig) == false)
    {
        return {R_FAILED, StringUtils::format("load project config file %s failed.", projectFile.c_str())};
    }
    if (projectConfig.apps.empty())
    {
        return {R_FAILED, "app cannot be empty."};
    }

    auto result = buildModule(projectConfig.name, projectFolder, projectFolderByBuild, &projectConfig);
    if (result.isError())
    {
        return result;
    }

    result = oneExplainer.generate();
    if (result.isError())
    {
        return result;
    }

    ProjectConfig::App& app = projectConfig.apps.front();
    result = cppGenerator.generate(app.exeName, app.mainClass, FileUtils::appendFileName(folder, "build"));
    if (result.isError())
    {
        return result;
    }

    return {};
}
    
Result Project::buildModule(const string& name, const string& folder, const string& folderByBuild, ProjectConfig* config)
{
    if (buildedModules.count(name) > 0)
    {
        return {};
    }
    buildedModules.insert(name);

    //folder是现可访问的目录。要转换成build的相对目录
    string oneFolder = FileUtils::appendFileName(folder, "one");
    auto result = oneExplainer.explain(oneFolder, "one");
    if (result.isError())
    {
        return result;
    }

    //添加原生
    for (auto& native : config->natives)
    {
        cppGenerator.addNativeClass(native.oneClass, native.hPath, native.cppClass);
    }

    //添加头文件目录
    string includeFolder = FileUtils::appendFileName(folderByBuild, "include");
    cppGenerator.addIncludeFolder(includeFolder);
    for (auto includeFolder2 : config->includeFolders)
    {
        if (FileUtils::isRelativePath(includeFolder2))
        {
            includeFolder2 = FileUtils::appendFileName(includeFolder, includeFolder2);
        }
        cppGenerator.addIncludeFolder(includeFolder2);
    }

    //添加库
    string libFolder = FileUtils::appendFileName(folderByBuild, "lib");
    cppGenerator.addLibraryFolder(libFolder);
    for (auto& lib : config->libs)
    {
        cppGenerator.addLibrary(lib);
    }

    //编译依赖模块
    for (auto& dependModule : config->depends)
    {
        string moduleFolder;
        string moduleFolderByBuild;
        if (searchModule(&dependModule, moduleFolder, moduleFolderByBuild) == false)
        {
            return {R_FAILED, StringUtils::format("not found %s module.", dependModule.name)};
        }
        
        string moduleFile = FileUtils::appendFileName(moduleFolder, "one.json");
        ProjectConfig moduleConfig;
        if (ProjectConfigLoader::load(moduleFile, &moduleConfig) == false)
        {
            return {R_FAILED, StringUtils::format("load module config file %s failed.", moduleFile.c_str())};
        }
        
        auto result = buildModule(dependModule.name, moduleFolder, moduleFolderByBuild, &moduleConfig);
        if (result.isError())
        {
            return result;
        }
    }

    return {};
}
    
bool Project::searchModule(ProjectConfig::Depend* depend, string& folder, string& folderByBuild)
{
    string name = depend->name + "V" + depend->version;
    string path = FileUtils::appendFileName(FileUtils::appendFileName(projectFolder, "depend"), name);
    if (FileUtils::isDir(path))
    {
        folder = path;
        folderByBuild = FileUtils::appendFileName(FileUtils::appendFileName(projectFolderByBuild, "depend"), name);
        return true;
    }
    for (auto dependFolder : projectConfig.dependFolders)
    {
        string dependFolderByBuild = dependFolder;
        if (FileUtils::isRelativePath(dependFolder))
        {
            dependFolderByBuild = FileUtils::appendFileName(projectFolderByBuild, dependFolder);
            dependFolder = FileUtils::appendFileName(projectFolder, dependFolder);
        }
        path = FileUtils::appendFileName(dependFolder, name);
        if (FileUtils::isDir(path))
        {
            folder = path;
            folderByBuild = FileUtils::appendFileName(dependFolderByBuild, name);
            return true;
        }
    }
    return false;
}
