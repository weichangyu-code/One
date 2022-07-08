#include "ProjectConfigLoader.h"
#include "FileUtils.h"
using namespace OneCommon;
#include "json/reader.h"

bool ProjectConfigLoader::load(const string& path, ProjectConfig* project)
{
    string json = FileUtils::readFile(path);
    if (json.empty())
    {
        return false;
    }
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(json, value) == false)
    {
        return false;
    }
    project->name = value["name"].asString();
    if (project->name.empty())
    {
        return false;
    }
    project->version = value["version"].asString();
    for (auto& dependValue : value["depend"])
    {
        ProjectConfig::Depend depend;
        depend.name = dependValue["name"].asString();
        depend.version = dependValue["version"].asString();
        project->depends.push_back(std::move(depend));
    }
    for (auto& nativeValue : value["native"])
    {
        ProjectConfig::Native native;
        native.oneClass = nativeValue["oneClass"].asString();
        native.hPath = nativeValue["hPath"].asString();
        native.cppClass = nativeValue["cppClass"].asString();
        project->natives.push_back(std::move(native));
    }
    for (auto& appValue : value["app"])
    {
        ProjectConfig::App app;
        app.exeName = appValue["exeName"].asString();
        app.mainClass = appValue["mainClass"].asString();
        project->apps.push_back(std::move(app));
    }
    return true;
}