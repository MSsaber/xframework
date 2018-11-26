#include "stdafx.h"
#include "xfNode.h"

using namespace XFRAME;
using namespace std;

Node::Node()
{
	ParentNode = nullptr;
	NextNode = nullptr;
}

Node::~Node()
{
	if (DType == Node::All)
	{
		for (auto node : ChildrenNode)
		{
			delete node;
			node = nullptr;
		}
	}
}

Node* Node::GetChild(XUINT index)
{
	return index < ChildrenNode.size() ? ChildrenNode[index] : nullptr;
}

void Node::SetRecycleType(DestructType type)
{
	DType = type;
}

Node* Node::GetParent()
{
	return ParentNode;
}

void Node::SetParent(Node* node)
{
	this->ParentNode = node;
}

const Node* Node::GetParent() const
{
	return ParentNode;
}

void Node::AddChild(Node* node)
{
	ChildrenNode.size() ? ChildrenNode[ChildrenNode.size() - 1]->NextNode = node : nullptr;
	ChildrenNode.push_back(node);
	node->ParentNode = this;
}

bool Node::InsertChild(XUINT pos, Node* node)
{
	if (ChildrenNode.size() > pos)
	{
		ChildrenNode.insert(ChildrenNode.begin() + pos, node);
		return true;
	}
	return false;
}

void Node::RemoveChild(Node* node, bool recycleChildren)
{
	for (NodeVector::iterator iter = ChildrenNode.begin();
		iter != ChildrenNode.end();)
	{
		if (*iter == node)
		{
			iter = ChildrenNode.erase(iter);
			if (recycleChildren)
			{
				delete node;
				node = nullptr;
			}
			return;
		}
		else
		{
			iter++;
		}
	}
}

NodeVector& Node::GetChildren()
{
	return ChildrenNode;
}



void Node::Traverse(Node* node, TraverseDelegate td)
{
	Node* tnode = node;
	while (!td(tnode))
	{
		tnode++;
	}
}

//operator 
Node* Node::operator++()
{
	if (!GetParent() && !GetChildren().size())
	{
		return nullptr;
	}
	if (this->NextNode)
	{
		return this->NextNode;
	}
	if (!this->NextNode &&
		!GetParent() &&
		GetChildren().size())
	{
		return GetChildren()[0];
	}
	if (!this->NextNode && 
		GetParent()->NextNode &&
		GetParent()->NextNode->GetChildren().size())
	{
		return GetParent()->NextNode->GetChildren()[0];
	}
	if (!this->NextNode && 
		!GetParent()->NextNode &&
		GetParent()->GetChildren()[0]->GetChildren().size())
	{
		return  this->GetParent()->GetChildren()[0]->GetChildren()[0];
	}
	return nullptr;
}

Node* Node::operator<<(Node* other)
{
	this->AddChild(other);
	return this;
}

Node* Node::operator>>(Node* other)
{
	this->RemoveChild(other, true);
	return this;
}