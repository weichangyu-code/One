#pragma once
#include "ProjectConfig.h"

class ProjectConfigLoader
{
public:
    static bool load(const string& path, ProjectConfig* project);
};