#pragma once
#include "Common.h"
#include "../common/Result.h"
#include "../explainer/OneExplainer.h"
#include "../generator/CppGenerator.h"
#include "ProjectConfig.h"

class Project
{
public:
    Project();

    Result build(const string& folder);

protected:
    Result buildModule(const string& name, const string& folder, const string& folderByBuild, ProjectConfig* config);
    bool searchModule(ProjectConfig::Depend* depend, string& folder, string& folderByBuild);

protected:
    string projectFolder;
    string projectFolderByBuild;
    ProjectConfig projectConfig;

    OneExplainer oneExplainer;
    CppGenerator cppGenerator;

    set<string> buildedModules;
};
