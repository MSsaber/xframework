#ifndef _XFTASKPOOL_H_
#define _XFTASKPOOL_H_

#include "../xfTypeDef.h"

namespace XFRAME
{
	class TaskPool
	{
		TaskPool(const TaskPool&) = delete;
		TaskPool(TaskPool&&) = delete;
		TaskPool& operator=(const TaskPool&) = delete;
		TaskPool&& operator=(TaskPool&&) = delete;
	public:
		TaskPool();
		TaskPool(XUINT maxThreadCount);
		~TaskPool();

	private:
		XUINT MaxCount;
	};
}

#endif //!_XFTASKPOOL_H_
