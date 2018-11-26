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
	class Node : public Object
	{
		friend class  Node;
	public:
		typedef std::function<bool(Node*)> TraverseDelegate;
		enum DestructType
		{
			All,         //Recycle all resources,including child nodes
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
	public:
		Node* GetChild(XUINT index);
		void    AddChild(Node* node);
		bool    InsertChild(XUINT pos, Node* node);
		void    RemoveChild(Node* node, bool recycleChildren);

	public:
		/* traversing*/
		Node* operator++();

		/*Add child node*/
		Node* operator<<(Node* other);

		/*remove child node*/
		Node* operator>>(Node* other);
	public:
		/**Return turn, stop traversing  */
		static  void  Traverse(Node* node, TraverseDelegate td);
	private:
		NodeVector   ChildrenNode;
		Node*            ParentNode;
		Node*            NextNode;
		DestructType DType;
	};
}


#endif //_XFNODE_H_
