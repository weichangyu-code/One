#pragma once
#include "Common.h"

class ProjectConfig
{
public:
    class Depend
    {
    public:
        string name;
        string version;
    };
    class Native
    {
    public:
        string oneClass;
        string hPath;
        string cppClass;
    };
    class App
    {
    public:
        string exeName;
        string mainClass;
    };

public:
    string name;
    string version;
    list<Depend> depends;
    list<Native> natives;
    list<App> apps;
};
