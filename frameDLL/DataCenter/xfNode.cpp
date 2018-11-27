#include "stdafx.h"
#include "combaseapi.h"
#include "xfNode.h"

using namespace XFRAME;
using namespace std;

std::string __fastcall GetUUid()
{
	char buffer[64] = { 0 };
	GUID guid;

	if (CoCreateGuid(&guid))
	{
		fprintf(stderr, "create guid error\n");
		return "";
	}
	snprintf(buffer, sizeof(buffer),
		"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return std::string(buffer);
}


Node::Node()
{
	std::string uuid_str = GetUUid();
	if (uuid_str.length())
	{
		NodeId = uuid_str;
	}
	ParentNode = nullptr;
	NextNode = nullptr;
}

Node::~Node()
{
	RemoveAll(DType == Node::All ? true : false);
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

void Node::SetNodeName(const std::string& name)
{
	NodeName = name;
}

std::string Node::GetNodeName()
{
	return NodeName;
}

void Node::SetNodeInfo(const std::string& info)
{
	NodeInfo = info;
}

std::string Node::GetNodeInfo()
{
	return NodeInfo;
}

std::string Node::GetNodeId()
{
	return NodeId;
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

void Node::RemoveChildren(XUINT pos, XUINT count)
{
	XUINT index = 0;
	for (NodeVector::iterator iter = ChildrenNode.begin();
		iter != ChildrenNode.end(); )
	{
		if (index >= pos && index <= count)
		{
			iter = ChildrenNode.erase(iter);
		}
		else
		{
			iter++;
		}
		index++;
	}
}

void Node::RemoveAll(bool recycleChildren)
{
	if (recycleChildren)
	{
		for (auto node : ChildrenNode)
		{
			delete node;
			node = nullptr;
		}
	}
	ChildrenNode.clear();
}

NodeVector& Node::GetChildren()
{
	return ChildrenNode;
}

const NodeVector& Node::GetChildren() const
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