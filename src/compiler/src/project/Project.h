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
    Result buildModule(const string& name, const string& folder, ProjectConfig* config);
    string searchModule(ProjectConfig::Depend* depend);

protected:
    string projectFolder;
    ProjectConfig projectConfig;

    OneExplainer oneExplainer;
    CppGenerator cppGenerator;

    set<string> buildedModules;
};
