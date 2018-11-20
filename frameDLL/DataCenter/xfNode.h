#ifndef _XFNODE_H_
#define _XFNODE_H_

#include <vector>
#include "xfObject.h"

namespace XFRAME
{

	class Node : public Object
	{
	public:
		typedef std::vector<Node*> NodeVector;

		enum DestructType
		{
			All,    //Recycle all resources,including child nodes
			Oneself //Just recycle self
		};
	public:
		Node();
		virtual ~Node();

	public:
		Node* GetChild(XUINT index);

	private:
		NodeVector   ChildrenNode;
		DestructType DType;
	};
}


#endif //_XFNODE_H_
