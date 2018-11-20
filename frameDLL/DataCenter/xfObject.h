#ifndef _XFOBJECT_H_
#define _XFOBJECT_H_

#include "../Base/xfRuntimeClass.h"

namespace XFRAME
{

	class Object
	{
	public:
		virtual PRuntimeClass GetRuntimeClass() const;

		bool IsKindOf(const RuntimeClass* runtimeClass) const;
	protected:
		Object();
		virtual ~Object() = default;
	public:
		static RuntimeClass m_ObjectClass;

		static PRuntimeClass GetThisClass();
		static PRuntimeClass GetBaseClass();
	};
}

#endif //_XFOBJECT_H_
