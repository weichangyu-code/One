#include "Common.h"
#include <fstream>

class OneOStream
{
public:
    OneOStream(const char* fileName)
        :file(fileName)
    {

    }

public:
    void writeKeywords(const vector<string>& keywords)
    {
        file << "const char* OneRuleMap::oneKeywords[] = {";
        for (auto& keyword : keywords)
        {
            file << "\"" << keyword << "\", ";
        }
        file << "nullptr};\n\n";
    }
    void writeRuleNames(const vector<string>& ruleNames)
    {
        file << "const char* OneRuleMap::oneRuleNames[] = {";
        for (auto& name : ruleNames)
        {
            file << "\"" << name << "\", ";
        }
        file << "nullptr};\n\n";
    }

    void startWriteRuleMap()
    {
        file << "const int OneRuleMap::oneRuleMap[] = {";
    }

    OneOStream& operator << (int i)
    {
        char buf[16] = {0};
        sprintf(buf, "0x%x", i);
        file << buf << ", ";
        return *this;
    }

    void endWriteRuleMap()
    {
        file << "0x0};\n\n";
    }

protected:
    int ruleMapSize = 0;
    ofstream file;
};

class OneIStream
{
public:
    OneIStream(const int* source)
        : source(source)
    {

    }
    
    inline OneIStream& operator >> (int& i)
    {
        i = *source;
        source++;
        return *this;
    }

protected:
    const int* source;
};
