#ifndef _XFTASKPOOL_H_
#define _XFTASKPOOL_H_

#include <map>
#include "../xfTypeDef.h"
#include "xfTask.h"

namespace XFRAME
{
	class xframe_pai TaskPool
	{
		typedef std::map<std::string, void*>::iterator TaskIter;

		TaskPool(const TaskPool&) = delete;
		TaskPool(TaskPool&&) = delete;
		TaskPool& operator=(const TaskPool&) = delete;
		TaskPool&& operator=(TaskPool&&) = delete;
	public:
		TaskPool();
		TaskPool(XUINT maxThreadCount);
		~TaskPool();
	public:
		template<typename Future_type,
		typename Fx,
		typename... Types>
		bool AddTask(bool IsAysn, const std::string& Identify, Fx&& Func, Types&&... Args) {
			if (IsTaskExist(Identify))
			{
				return false;
			}

			Tasks.insert(std::make_pair(Identify,
				(void*)new Task<Future_type>(IsAysn,
					std::move(std::forward<Fx>(Func)),
					std::move(std::forward<Types>(Args))...)));

			return true;
		}

		template<typename Future_type>
		Future_type GetResult(const std::string& Identify) {
			TaskIter iter = Tasks.find(Identify);
			if (iter == Tasks.end())
			{
				return (Future_type)0;
			}
			Task<Future_type>* tsk =
				static_cast<Task<Future_type>*>(iter->second);
			return tsk->GetResult();
		}

		inline bool IsTaskExist(const std::string& Identify) {
			return Tasks.find(Identify) != Tasks.end();
		}
	public:
		void Joinall();

		void GetResult(const std::string& Identify);

		void DeleteTask(const std::string& Identify);

		bool IsTaskDone(const std::string& Identidy);

		bool IsAutoManager();

		void SetAutoManager(bool);

		/*Remove completed tasks*/
		void OrganizingTasks();
	private:
		void Manager();
	private:
		XUINT                        MaxCount;  // Valid when the thread pool is automatically managed
		std::unique_ptr<Task<void>>  ManagerTask;
		std::map<std::string, void*> Tasks;
		std::atomic<bool>            AutoManager;
	};
}

#endif //!_XFTASKPOOL_H_
