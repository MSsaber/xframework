#ifndef _XFRUNTIMECLASS_H_
#define _XFRUNTIMECLASS_H_

#include "../xfTypeDef.h"

namespace XFRAME
{
	class  Object;
	
	typedef struct xframe_pai RuntimeClass
	{
	public:		
		XCPSTR m_TypeName;
		int    m_TypeSize;
		XUINT  m_Version;

		Object* (*m_Construction)();

	public:	
		static RuntimeClass *GetByName(XCPSTR name);
		static Object *CreateObject(const RuntimeClass* runtimeClass);
	public:
		bool operator==(const RuntimeClass& other);

		bool IsDerivedFrom(const RuntimeClass* baseClass) const;
		bool IsSameBase(const RuntimeClass* otherClass) const;

		RuntimeClass *GetThisClass();
		RuntimeClass *GetBaseClass();

	}RuntimeClass, *PRuntimeClass;

#define _CHECK_RUNTIMECLASS(ThisClass,OtherClass) \
	(*XFRAME::RuntimeClass::GetByName(ThisClass) == *XFRAME::RuntimeClass::GetByName(OtherClass))
#define CHECK_RUNTIMECLASS(ThisClass,OtherClass) \
	_CHECK_RUNTIMECLASS(#ThisClass,#OtherClass)

	template<typename _To,typename _From>
	_To* xdynamic_cast(const _From* ptr){
		if (!ptr){
			//invaild pointer
			return nullptr;
		}
		//TODO : Pending completion
		return ptr->IsKindOf() ? static_cast<_To*>(ptr) : nullptr;
	}
}

#endif //_XFRUNTIMECLASS_H_
