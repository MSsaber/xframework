#include "stdafx.h"

#include "../Base/xfRuntimeClassTable.h"

#include "xfObject.h"

using namespace XFRAME;

RuntimeClass Object::m_ObjectClass{ "Object",sizeof(Object),1,nullptr };

Object::Object()
{

}

bool Object::IsKindOf(const RuntimeClass* runtimeClass) const
{
	if (!runtimeClass)
	{
		//invaild RuntimeClass pointer
		return false;
	}
	return *GetRuntimeClass() == *runtimeClass;
}

PRuntimeClass Object::GetRuntimeClass() const
{
	return &m_ObjectClass;
}

PRuntimeClass Object::GetThisClass()
{
	return RuntimeClassTable::GetRuntimeClass(m_ObjectClass.m_TypeName);
}

PRuntimeClass Object::GetBaseClass()
{
	return m_ObjectClass.GetBaseClass();
}