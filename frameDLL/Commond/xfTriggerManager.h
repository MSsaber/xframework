#ifndef _XFTRIGGER_MANAGER_H_
#define _XFTRIGGER_MANAGER_H_

#include <list>
#include <memory>

namespace XFRAME
{
	class xframe_pai TriggerManager
	{
		friend class CommondInterface;
	public:
		static bool Fire();
	public:
		static void AddCommondTrigger(CommondInterface*);
		static void RemoveCommondTrigger(CommondInterface*);
	private:
		static std::list<std::unique_ptr<CommondInterface>> CLArrey;
	private:
		TriggerManager() = delete;
		virtual ~TriggerManager() = delete;
		TriggerManager(const TriggerManager&) = delete;
		TriggerManager(TriggerManager&&) = delete;
		TriggerManager& operator=(const TriggerManager&) = delete;
	};

#ifndef RERISTER_COMMOND_BY_CLASSNAME
#define RERISTER_COMMOND_BY_CLASSNAME(ClassName) \
	TriggerManager::AddCommondTrigger(new ClassName);
#endif //!RERISTER_COMMOND_BY_CLASSNAME

}
#endif //!_TRIGGER_MANAGER_H_