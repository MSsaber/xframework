#ifndef _XFTASK_H_
#define _XFTASK_H_

#include <future>

#ifdef _WINDOW_OS_
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#endif //!_WINDOW_OS_

namespace XFRAME
{
	template<typename Future_type>
	class Task
	{
		typedef void* TaskHanle;
		typedef unsigned long TaskMemory;

		Task(const Task&) = delete;
		Task(Task&&) = delete;
		Task& operator=(const Task&) = delete;
		Task&& operator=(Task&&) = delete;
	public:
		template<typename _Fx, typename... _Types>
		explicit Task(bool IsAysn,_Fx&& _Func, _Types&&... _Args){
			this->IsAsyn = IsAsyn;
			auto func = std::bind(_STD forward<_Fx>(_Func), _STD forward<_Types>(_Args)...);
			if (IsAysn){
				std::packaged_task<Future_type(_Types...)> pTsk(func);
				Result = pTsk.get_future();
				Tsk = std::move(std::thread(std::move(pTsk)));
			}
			else {
				DoTask.store(false, std::memory_order_release);
				Tsk = std::move(std::thread([func,this]() {
					while (true)
					{
						if (DoTask.load(std::memory_order_acquire))
						{
							func;
							break;
						}
					}
				}));
			}
		}
		~Task() {
			if (!Controllable){
				return;
			}
			if (Tsk.joinable()){
				//Task done
				Tsk.join();
			}
			else if (IsForceClose){
				//Do not wait for the task to complete
				Tsk.join();
			}
			else{
				//Hand over tasks to system management
				Tsk.detach();
			}
		}
	public:
		void Run() {
			if (IsAsyn){
				Result.get();
			}
			else {
				DoTask.store(true, std::memory_order_release);
			}
		}

		void Join() {
			Tsk.join();
		}

		bool Joinable() {
			return Tsk.joinable();
		}

		void Detach() {
			Controllable = false;
			Tsk.detach();
		}

		TaskHanle GetHandle() {
			Tsk.native_handle();
		}

		TaskMemory GetTaskSize(){
#ifdef _WINDOW_OS_
			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo((HANDLE)Tsk.native_handle(), &pmc, sizeof(pmc));
			return pmc.WorkingSetSize;
#endif //!_WINDOW_OS_
			return 0;
		}

		Future_type GetResult() {
			return (_STD move(Result._Get_value()));
		}
	public:
		inline std::thread::id get_id() const {
			Tsk.get_id();
		}

		inline bool IsContrillable() const {
			return Controllable;
		}

		inline void SetRecyclingMethod(bool m) {
			IsForceClose = m;
		}

	private:
		bool                     Controllable = true;
		bool                     IsForceClose = false;
		bool                     IsAsyn;
		std::atomic<bool>        DoTask;
		std::future<Future_type> Result;
		std::thread              Tsk;
	};
}

#endif //!_XFTASK_H_
