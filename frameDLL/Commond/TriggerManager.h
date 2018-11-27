#ifndef _TRIGGER_MANAGER_H_
#define _TRIGGER_MANAGER_H_

#include <list>
#include <memory>

namespace XFRAME
{
	class CommondLine;
	class TriggerManager
	{
		friend class CommondLine;
	public:
		TriggerManager();
		virtual ~TriggerManager();
	public:
		static void AddCommondTrigger(CommondLine*);
		static void RemoveCommondTrigger(CommondLine*);
	private:
		static std::list<std::unique_ptr<CommondLine>> CLArrey;
	};

}
#endif //!_TRIGGER_MANAGER_H_