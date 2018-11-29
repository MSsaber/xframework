#include "stdafx.h"
#include "xfCommond.h"
#include "xfTriggerManager.h"

using namespace XFRAME;

std::list<std::unique_ptr<CommondInterface>> TriggerManager::CLArrey;

bool TriggerManager::Fire()
{
	return false;
}

void TriggerManager::AddCommondTrigger(CommondInterface* cl)
{
	if (!cl)
	{
		return;
	}
	CLArrey.emplace_back(std::move(std::unique_ptr<CommondInterface>(cl)));
}

void TriggerManager::RemoveCommondTrigger(CommondInterface* cl)
{
	for (std::list<std::unique_ptr<CommondInterface>>::iterator iter = CLArrey.begin();
		iter != CLArrey.end() ;iter++)
	{
		if (cl == (*iter).get())
		{
			*iter = nullptr;
		}
	}
}