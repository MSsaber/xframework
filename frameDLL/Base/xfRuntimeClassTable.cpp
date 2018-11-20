#include "stdafx.h"

#include "xfRuntimeClassTable.h"

using namespace XFRAME;
using namespace std;

void RuntimeClassTable::ResigterRuntimeClass(XFRAME::RuntimeClass& runtimeClass)
{
	_RuntimeClassTable* instance = Instance();
	if (!instance)
	{
		//invaild table instance
		return;
	}

	//resigter runtime class info
	instance->insert(make_pair(runtimeClass.m_TypeName, runtimeClass));
}

PRuntimeClass RuntimeClassTable::GetRuntimeClass(const std::string& name)
{
	_RuntimeClassTable* instance = Instance();
	if (!instance)
	{
		//invaild table instance
		return nullptr;
	}

	_RuntimeClassTable::iterator iter = instance->find(name);
	if (iter != instance->end())
	{
		return &(iter->second);
	}

	return nullptr;
}