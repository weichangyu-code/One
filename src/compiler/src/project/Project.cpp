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

    auto result = buildModule(projectConfig.name, projectFolder, &projectConfig);
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
    
Result Project::buildModule(const string& name, const string& folder, ProjectConfig* config)
{
    if (buildedModules.count(name) > 0)
    {
        return {};
    }
    buildedModules.insert(name);

    //
    string oneFolder = FileUtils::appendFileName(folder, "one");
    auto result = oneExplainer.explain(oneFolder, "one");
    if (result.isError())
    {
        return result;
    }

    //添加原生
    string includeFolder = FileUtils::appendFileName(folder, "include");
    if (FileUtils::isRelativePath(includeFolder))
    {
        //最终是生成在build目录下的，所以要提一层
        includeFolder = "../" + includeFolder;
    }
    for (auto& native : config->natives)
    {
        string path = FileUtils::appendFileName(includeFolder, native.hPath);
        cppGenerator.addNativeClass(native.oneClass, path, native.cppClass);
    }

    //添加头文件目录
    cppGenerator.addIncludeFolder(includeFolder);
    for (auto includeFolder : config->includeFolders)
    {
        if (FileUtils::isRelativePath(includeFolder))
        {
            includeFolder = FileUtils::appendFileName(folder, includeFolder);
            if (FileUtils::isRelativePath(includeFolder))
            {
                includeFolder = "../" + includeFolder;
            }
        }
        cppGenerator.addIncludeFolder(includeFolder);
    }

    //添加库
    string libFolder = FileUtils::appendFileName(folder, "lib");
    if (FileUtils::isRelativePath(libFolder))
    {
        libFolder = "../" + libFolder;
    }
    for (auto& lib : config->libs)
    {
        cppGenerator.addLibrary(FileUtils::appendFileName(libFolder, lib));
    }

    //编译依赖模块
    for (auto& dependModule : config->depends)
    {
        string moduleFolder = searchModule(&dependModule);
        if (moduleFolder.empty())
        {
            return {R_FAILED, StringUtils::format("not found %s module.", dependModule.name)};
        }
        
        string moduleFile = FileUtils::appendFileName(moduleFolder, "one.json");
        ProjectConfig moduleConfig;
        if (ProjectConfigLoader::load(moduleFile, &moduleConfig) == false)
        {
            return {R_FAILED, StringUtils::format("load module config file %s failed.", moduleFile.c_str())};
        }
        
        auto result = buildModule(dependModule.name, moduleFolder, &moduleConfig);
        if (result.isError())
        {
            return result;
        }
    }

    return {};
}
    
string Project::searchModule(ProjectConfig::Depend* depend)
{
    string name = depend->name + "V" + depend->version;
    string path = FileUtils::appendFileName(FileUtils::appendFileName(projectFolder, "depend"), name);
    if (FileUtils::isDir(path))
    {
        return path;
    }
    for (auto dependFolder : projectConfig.dependFolders)
    {
        if (FileUtils::isRelativePath(dependFolder))
        {
            dependFolder = FileUtils::appendFileName(projectFolder, dependFolder);
        }
        path = FileUtils::appendFileName(dependFolder, name);
        if (FileUtils::isDir(path))
        {
            return path;
        }
    }
    return "";
}
