#include "stdafx.h"
#include "xfTriggerManager.h"
#include "xfCommond.h"

using namespace XFRAME;

CommondInterface::CommondInterface()
{
	TriggerManager::AddCommondTrigger(this);
}

CommondInterface::~CommondInterface()
{
	TriggerManager::RemoveCommondTrigger(this);
}