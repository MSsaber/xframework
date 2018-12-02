#include "stdafx.h"
#include "combaseapi.h"
#include "xfNode.h"
#include "../Task/xfTaskPool.h"

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
	DType = Node::All;
	ParentNode = nullptr;
	NextNode = nullptr;
	PreNode = nullptr;
}

Node::~Node()
{
	ParentNode = nullptr;
	NextNode = nullptr;
	PreNode = nullptr;
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
	ChildrenNode.size() && ChildrenNode[ChildrenNode.size() - 1]->NextNode ?
		node->NextNode = ChildrenNode[ChildrenNode.size() - 1]->NextNode : nullptr;
	ChildrenNode.size() ? ChildrenNode[ChildrenNode.size() - 1]->NextNode = node : nullptr;

	if (ChildrenNode.size())
	{
		node->PreNode = ChildrenNode[ChildrenNode.size() - 1];
	}
	else if (PreNode)
	{
		Node* Pre = PreNode;
		while (Pre && !Pre->GetChildren().size())
		{
			Pre = Pre->PreNode;
		}
		if (Pre)
		{
			node->PreNode = Pre->GetChildren()[Pre->GetChildren().size() - 1];
			Pre->GetChildren()[Pre->GetChildren().size() - 1]->NextNode = node;
		}
	}
	if (NextNode && !node->NextNode)
	{
		Node* Nxt = NextNode;
		while (Nxt && !Nxt->GetChildren().size())
		{
			Nxt = Nxt->NextNode;
		}
		if (Nxt)
		{
			node->NextNode = Nxt->GetChildren()[0];
			Nxt->GetChildren()[0]->PreNode = node;
		}
	}

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
	Node::NodeIterator iter(node);
	while (!td(*iter))
	{
		iter++;
	}
}

Node::NodeIterator& 
Node::NodeIterator::operator=(
	const Node::NodeIterator& other)
{
	this->Value = other.Value;
	return *this;
}

Node* Node::NodeIterator::operator*()
{
	return this->Value;
}

Node::NodeIterator& 
Node::NodeIterator::operator++(int)
{
	if (!this->Value->GetParent() && 
		!this->Value->GetChildren().size() &&
		!this->Value->NextNode &&
		!this->Value->PreNode)
	{
		this->Value = nullptr;
	}
	else if (this->Value->NextNode)
	{
		this->Value = this->Value->NextNode;
	}
	else if (!this->Value->NextNode &&
		!this->Value->GetParent() &&
		this->Value->GetChildren().size())
	{
		this->Value = this->Value->GetChildren()[0];
	}
	else if (!this->Value->NextNode &&
		this->Value->GetParent()->NextNode &&
		!this->Value->GetChildren().size())
	{
		Node* LastLevelNode = this->Value->GetParent()->NextNode;
		while (LastLevelNode)
		{
			if (LastLevelNode->GetChildren().size())
			{
				this->Value = LastLevelNode->GetChildren()[0];
				return *this;
			}
			LastLevelNode = LastLevelNode->NextNode;
		}

		Node* ThisLevelFirstNode = this->Value;
		while (ThisLevelFirstNode)
		{
			Node* TempNode = ThisLevelFirstNode->PreNode;
			if (!TempNode)
			{
				break;
			}
			ThisLevelFirstNode = TempNode;
		}

		while (ThisLevelFirstNode)
		{
			if (ThisLevelFirstNode->GetChildren().size())
			{
				this->Value = ThisLevelFirstNode->GetChildren()[0];
				return *this;
			}
			ThisLevelFirstNode = ThisLevelFirstNode->NextNode;
		}
		this->Value = nullptr;
	}
	else if (!this->Value->NextNode &&
		!this->Value->GetParent()->NextNode &&
		this->Value->GetParent()->GetChildren()[0]->GetChildren().size())
	{
		this->Value = this->Value->GetParent()->GetChildren()[0]->GetChildren()[0];
	}
	else if ((!this->Value->NextNode &&
		!this->Value->GetParent()->NextNode &&
		!this->Value->GetChildren().size()) ||
		(!this->Value->NextNode &&
			this->Value->PreNode))
	{
		Node* ThisLevelFirstNode = this->Value;
		while (ThisLevelFirstNode)
		{
			Node* TempNode = ThisLevelFirstNode->PreNode;
			if (!TempNode)
			{
				break;
			}
			ThisLevelFirstNode = TempNode;
		}
		while (ThisLevelFirstNode)
		{
			if (ThisLevelFirstNode->GetChildren().size())
			{
				this->Value = ThisLevelFirstNode->GetChildren()[0];
				return *this;
			}
			ThisLevelFirstNode = ThisLevelFirstNode->NextNode;
		}
		this->Value = nullptr;
	}
	else
	{
		this->Value = nullptr;
	}
	return *this;
}

bool 
Node::NodeIterator::operator==(const Node::NodeIterator& other)
{
	return this->Value == other.Value;
}