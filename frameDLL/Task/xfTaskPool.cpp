#include "stdafx.h"

#include "xfTaskPool.h"

using namespace XFRAME;

TaskPool::TaskPool()
{
	MaxCount = 0;
}

TaskPool::TaskPool(XUINT maxThreadCount)
{
	MaxCount = maxThreadCount;
}

TaskPool::~TaskPool()
{
	Joinall();
}

void TaskPool::GetResult(const std::string& Identify) {
	TaskIter iter = Tasks.find(Identify);
	if (iter != Tasks.end())
	{
		Task<void>* tsk =
			(Task<void>*)iter->second;
		tsk->Run();
	}
}

void TaskPool::DeleteTask(const std::string& Identify)
{
	TaskIter iter = Tasks.find(Identify);
	if (iter != Tasks.end())
	{
		delete iter->second;
		iter->second = nullptr;
		Tasks.erase(iter);
	}
}

bool TaskPool::IsTaskDone(const std::string& Identidy)
{
	TaskIter iter = Tasks.find(Identidy);
	if (iter == Tasks.end())
	{
		return false;
	}
	
	if (!iter->second)
	{
		return false;
	}
	
	TaskInterface* ti = static_cast<TaskInterface*>(iter->second);
	return ti->Joinable();
}

bool TaskPool::IsAutoManager()
{
	return AutoManager.load(std::memory_order_acquire);
}

void TaskPool::SetAutoManager(bool t)
{
	AutoManager.store(t, std::memory_order_release);
	if (t && !ManagerTask)
	{
		ManagerTask = std::move(std::unique_ptr<Task<void>>(
			new Task<void>(false, std::bind(&TaskPool::Manager, this))));
		ManagerTask->Run();
	}
	else if (!t && ManagerTask)
	{
		ManagerTask = nullptr;
	}
}

void TaskPool::OrganizingTasks()
{
	TaskIter iter = Tasks.begin();
	while (iter != Tasks.end())
	{
		if (!iter->second)
		{
			//invaild task pointer
			continue;
		}
		TaskInterface* ti = static_cast<TaskInterface*>(iter->second);
		if (ti->Joinable())
		{
			delete iter->second;
			iter->second = nullptr;
			iter = Tasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TaskPool::Joinall()
{
	TaskIter iter = Tasks.begin();
	while (iter != Tasks.end())
	{
		if (!iter->second)
		{
			//invaild task pointer
			continue;
		}
		TaskInterface* ti = static_cast<TaskInterface*>(iter->second);
		delete iter->second;
		iter->second = nullptr;
		iter++;
	}
	Tasks.clear();
}

void TaskPool::Manager()
{
	while (AutoManager.load(std::memory_order_acquire))
	{
		OrganizingTasks();
	}
}