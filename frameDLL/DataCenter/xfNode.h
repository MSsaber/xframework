#ifndef _XFNODE_H_
#define _XFNODE_H_

#include <vector>
#include <functional>
#include "../xfTypeDef.h"
#include "xfObject.h"

namespace XFRAME
{
	class Node;
	typedef std::vector<Node*> NodeVector;
	class xframe_pai Node : public Object
	{
		friend class  Node;
		friend class NodeIterator;
	public:
		class xframe_pai NodeIterator
		{
		public:
			NodeIterator(Node* value) : Value(value) {}
			~NodeIterator() { Value = nullptr; }

			NodeIterator& operator=(const NodeIterator& other);
			Node* operator*();
			NodeIterator& operator++(int);
		private:
			Node* Value;
		};
	public:
		typedef std::function<bool(Node*)> TraverseDelegate;
		enum DestructType
		{
			All,    //Recycle all resources,including child nodes
			Oneself //Just recycle self
		};
	public:
		Node();
		virtual ~Node();
	public:
		void SetRecycleType(DestructType type);

		void     SetParent(Node* node);
		Node* GetParent();
		const Node* GetParent() const;

		NodeVector& GetChildren();
		const NodeVector& GetChildren() const;

		void SetNodeName(const std::string& name);
		std::string GetNodeName();

		void SetNodeInfo(const std::string& info);
		std::string GetNodeInfo();

		std::string GetNodeId();
	public:
		Node* GetChild(XUINT index);
		void    AddChild(Node* node);
		bool    InsertChild(XUINT pos, Node* node);
		void    RemoveChild(Node* node, bool recycleChildren);
		void    RemoveChildren(XUINT pos, XUINT count);
		void    RemoveAll(bool recycleChildren);

	public:
		NodeVector::iterator GetBeginIterator() { return ChildrenNode.size() ? ChildrenNode.begin() : ChildrenNode.end(); }
		NodeVector::iterator GetEndIterator() { return ChildrenNode.end(); }
		NodeVector::const_iterator GetConstBeginIterator() { return ChildrenNode.size() ? ChildrenNode.begin() : ChildrenNode.end(); }
		NodeVector::const_iterator GetConstEndIterator() { return ChildrenNode.end(); }
	public:
		/**Return turn, stop traversing  */
		static  void  Traverse(Node* node, TraverseDelegate td);
	private:
		std::string       NodeName;
		std::string       NodeInfo;
		std::string       NodeId;           //uuid
		NodeVector   ChildrenNode;
		Node*            ParentNode;
		Node*            NextNode;
		Node*            PreNode;
		DestructType DType;
	};
}


#endif //_XFNODE_H_
