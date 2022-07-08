#pragma once

class SyntaxBase;
class SyntaxFile;
class ExplainContext
{
public:
    ExplainContext();
    ExplainContext(SyntaxFile* file);
    ~ExplainContext();

    void startSyntax();
    SyntaxFile* finishSyntax();

    //
    void addSyntaxObject(SyntaxBase* obj);

public:
    SyntaxFile* file = nullptr;
};
