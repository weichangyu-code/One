#include "CppFile.h"
#include "../../explainer/meta/MetaFile.h"

CppFile::CppFile(MetaFile* metaFile, CppContainer* cppContainer)
    :CppBase(cppContainer)
{
    this->metaFile = metaFile;
    metaFile->ptr = this;
}
  
CppFile* CppFile::getCppFile(MetaFile* metaFile)
{
    return (CppFile*)metaFile->ptr;
}
