#pragma once

typedef int STMTLine;
#include <vector>
#include "TNode.h"

class Parent
{
public:
	Parent(void);
	~Parent(void);

	bool setParent(STMTLine parent, STMTLine child);
	bool isParent(STMTLine first, STMTLine second);
	bool isParentStar(STMTLine first, STMTLine second);
	TNode getParent(STMTLine child);
	vector<TNode> getParentStar(STMTLine child);
	vector<TNode> getChildOf(STMTLine parent);
	vector<TNode> getChildOfStar(STMTLine parent);


};

