#include "stdafx.h"
#include "Commond.h"
#include "TriggerManager.h"

using namespace XFRAME;

std::list<std::unique_ptr<CommondLine>> TriggerManager::CLArrey;

TriggerManager::TriggerManager()
{
}

TriggerManager::~TriggerManager()
{

}

void TriggerManager::AddCommondTrigger(CommondLine* cl)
{
	if (!cl)
	{
		return;
	}
	CLArrey.emplace_back(std::move(std::unique_ptr<CommondLine>(cl)));
}

void TriggerManager::RemoveCommondTrigger(CommondLine* cl)
{
	for (std::list<std::unique_ptr<CommondLine>>::iterator iter = CLArrey.begin();
		iter != CLArrey.end() ;iter++)
	{
		if (cl == (*iter).get())
		{
			*iter = nullptr;
		}
	}
}