#ifndef _XFTASKINTERFACE_H_
#define _XFTASKINTERFACE_H_

namespace XFRAME
{
	struct TaskInterface
	{
		virtual void Run() = 0;
		virtual void Join() = 0;
		virtual bool Joinable() = 0;
		virtual void Detach() = 0;
		virtual void* GetHandle() = 0;
		virtual unsigned long GetTaskSize() = 0;
	}; 
}

#endif //!_XFTASKINTERFACE_H_
