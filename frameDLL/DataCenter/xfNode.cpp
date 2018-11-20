#include "stdafx.h"

#include "xfNode.h"

using namespace XFRAME;
using namespace std;

Node::Node()
{
}

Node::~Node()
{
}

Node* Node::GetChild(XUINT index)
{
	return index < ChildrenNode.size() ? ChildrenNode[index] : nullptr;
}