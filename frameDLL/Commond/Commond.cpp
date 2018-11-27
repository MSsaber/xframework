#include "stdafx.h"
#include "TriggerManager.h"
#include "Commond.h"

using namespace XFRAME;

CommondLine::CommondLine()
{
	TriggerManager::AddCommondTrigger(this);
}

CommondLine::~CommondLine()
{
	TriggerManager::RemoveCommondTrigger(this);
}