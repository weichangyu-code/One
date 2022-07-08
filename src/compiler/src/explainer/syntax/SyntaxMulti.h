#pragma once

template<class T>
class SyntaxMulti : public SyntaxBase
{
public:
    SyntaxMulti(ExplainContext* context)
        :SyntaxBase(context)
    {
    }

    bool exist(const T& item)
    {
        return std::find(items.begin(), items.end(), item) != items.end();
    }

public:
    list<T> items;
};
