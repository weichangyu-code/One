#pragma once

#include "../common/Element.h"
#include "Common.h"

class Rule
{
public:
	Rule() = default;
	Rule(int index, string name, int nameIndex, string alias, int aliasIndex, int priority)
	{
		this->index = index;
		this->priority = priority;
		this->name = name;
		this->nameIndex = nameIndex;
		this->alias = alias;
		this->aliasIndex = aliasIndex;
	}

	bool validate() const
	{
		return (name.empty() == false) && (elements.empty() == false);
	}

public:
	int index = 0;
	int priority = 0;
	string name;
	int nameIndex = 0;
	string alias;
	int aliasIndex = 0;
	vector<Element> elements;
};