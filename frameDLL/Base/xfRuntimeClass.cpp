#include "stdafx.h"
#include "../DataCenter/xfObject.h"
#include "xfRuntimeClass.h"

using namespace XFRAME;
using namespace std;

PRuntimeClass RuntimeClass::GetByName(XCPSTR name)
{
	return nullptr;
}

Object *RuntimeClass::CreateObject(const RuntimeClass* runtimeClass)
{
	return nullptr;
}

PRuntimeClass RuntimeClass::GetThisClass()
{
	return nullptr;
}

PRuntimeClass RuntimeClass::GetBaseClass()
{
	return nullptr;
}

bool RuntimeClass::operator==(const RuntimeClass& other)
{
	if (m_TypeSize == other.m_TypeSize &&
		m_Version == other.m_Version &&
		XCPSTRCMP(m_TypeName,other.m_TypeName))
	{
		return true;
	}

	return false;
}