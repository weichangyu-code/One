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
    //加载工程配置文件
    string projectFile = FileUtils::appendFileName(folder, "one.json");
    ProjectConfig projectConfig;
    if (ProjectConfigLoader::load(projectFile, &projectConfig) == false)
    {
        return {R_FAILED, StringUtils::format("load project config file %s failed.", projectFile.c_str())};
    }
    if (projectConfig.apps.empty())
    {
        return {R_FAILED, "app cannot be empty."};
    }

    auto result = buildOne(folder, &projectConfig);
    if (result.isError())
    {
        return result;
    }

    string dependFolder = FileUtils::appendFileName(folder, "depend");
    for (auto& depend : projectConfig.depends)
    {
        string moduleFolder = FileUtils::appendFileName(dependFolder, depend.name + "V" + depend.version);
        string moduleFile = FileUtils::appendFileName(moduleFolder, "one.json");
        ProjectConfig moduleConfig;
        if (ProjectConfigLoader::load(moduleFile, &moduleConfig) == false)
        {
            return {R_FAILED, StringUtils::format("load module config file %s failed.", moduleFile.c_str())};
        }
        
        result = buildOne(moduleFolder, &moduleConfig);
        if (result.isError())
        {
            return result;
        }
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
    
Result Project::buildOne(const string& folder, ProjectConfig* config)
{
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
    cppGenerator.addIncludeFolder(FileUtils::appendFileName(folder, "include"));

    return {};
}
