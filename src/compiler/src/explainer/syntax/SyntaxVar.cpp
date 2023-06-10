#include "SyntaxVar.h"
#include "SyntaxType.h"

void SyntaxVar::addItem(const string& name, ExplainContext* context)
{
    items.push_back(new SyntaxTypePathItem(name, context));
}

