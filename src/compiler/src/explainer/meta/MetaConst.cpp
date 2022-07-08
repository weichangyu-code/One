#include "MetaConst.h"
#include "MetaContainer.h"

MetaType MetaConst::getType()
{
    if (type == DT_STRING)
    {
        return MetaType(metaContainer->getStringClass());
    }
    else
    {
        return MetaType(type);
    }
}
