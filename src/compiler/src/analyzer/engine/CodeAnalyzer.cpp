#include "CodeAnalyzer.h"

list<LexElement> CodeAnalyzer::analysis(const string& str)
{
    LexAnalyzer codeAnalyzer;
    list<LexElement> elements = codeAnalyzer.analysis(str);
    for (auto& element : elements)
    {
        if (element.type == Element::IDENTIFIER)
        {
            int index = block->getKeywordIndex(element.remark);
        	if (index >= 0)
        	{
        		element.type = Element::KEYWORD;
        		element.value = index;
        	}
        }
    }
    return elements;
}