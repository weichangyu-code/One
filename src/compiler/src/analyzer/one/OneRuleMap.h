#pragma once
#include "../map/RuleMap.h"
#include "OneStream.h"

class OneRuleMap : public RuleMap
{
public:
    void initOne();

    void generateInitOne(const char* fileName);

protected:
    void saveStrings(OneOStream& stream);
    void loadStrings(OneIStream& stream);
    void saveRules(OneOStream& stream);
    void loadRules(OneIStream& stream);
    void saveNodes(OneOStream& stream);
    void loadNodes(OneIStream& stream);
    void saveLinks(OneOStream& stream);
    void loadLinks(OneIStream& stream);
    void saveNodeLinks(OneOStream& stream);
    void loadNodeLinks(OneIStream& stream);

protected:
    static const char* oneKeywords[];
    static const char* oneRuleNames[];
    static const int   oneRuleMap[];

    // const char* OneRuleMap::oneKeywords[] = {"", nullptr};
    // const char* OneRuleMap::oneRuleNames[] = {"", nullptr};
    // const int   OneRuleMap::oneRuleMap[] = {0x00};
};