#pragma once
#include "Common.h"
#include "../common/Result.h"
#include "../explainer/OneExplainer.h"
#include "../generator/CppGenerator.h"

class Project
{
public:
    Project();

    Result build(const string& folder);

protected:
    Result buildOne(const string& folder, ProjectConfig* config);

protected:
    OneExplainer oneExplainer;
    CppGenerator cppGenerator;
};
