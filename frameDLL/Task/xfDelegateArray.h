#ifndef _XFDELEGATEARRAY_H_
#define _XFDELEGATEARRAY_H_

#include <list>
#include <queue>
#include <atomic>
#include <functional>

namespace XFRAME
{
	template<typename Fx>
	class DelegateArray
	{
		using Delegate = std::function<Fx>;
		using Dlist = typename std::list<std::pair<std::string, Delegate>>;

		DelegateArray(const DelegateArray&) = delete;
		DelegateArray(DelegateArray&&) = delete;
		DelegateArray& operator=(const DelegateArray&) = delete;
		DelegateArray&& operator==(DelegateArray&&) = delete;
	public:
		DelegateArray(bool isThread) : IsThread(isThread) {
			InitArray();
		}
		~DelegateArray() {
			Destory();
		};
	public:
		inline void SetThreadSwitch(bool isThread) {
			IsThread = isThread;
		}

		inline bool IsThreadType() {
			return IsThread;
		}

		inline void PushDelegate(const std::string& eventName, Delegate dg) {
			Dlist& dlist = Load();
			dlist.push_back(std::make_pair(eventName, dg));
			Store();
		}

		Delegate Front() {
			Dlist& dlist = Load();
			if (dlist.size())
			{
				return (*dlist.begin()).second;
			}
			return 0;
		}

		void Pop() {
			Dlist& dlist = Load();
			if (dlist.size())
			{
				dlist.erase(dlist.begin());
				Store();
			}
		}

		Delegate GetDelegate(const std::string& eventName) {
			Dlist& dlist =Load();
			for (auto dg : dlist){
				if (dg.first == eventName){
					return dg.second;
				}
			}
			return 0;
		}

		void RemoveDelegate(const std::string& eventName) {
			Dlist& dlist = Load();
			dlist.remove_if([eventName](std::pair<std::string, Delegate>& val) {
				if (eventName == val.first){
					return true;
				}
				else {
					return false;
				}
			});
			Store();
		}

		inline bool Empty() {
			Dlist& dlist = Load();
			return dlist.empty();
		}

		template<class T,
			typename Future_type,
			typename... Types>
		inline void Bind(const std::string& eventName, Future_type(T::*func)(Types...), T* ptr, Types&&... args) {
			PushDelegate(eventName, std::bind(func, ptr, args...));
		}

		template<typename... Types>
		inline void Bind(const std::string& eventName, Delegate dg, Types&&... args) {
			PushDelegate(eventName, std::bind(dg, args...));
		}


	private:
		inline void InitArray() {
			if (IsThread){
				AtomicList.store(&List, std::memory_order_release);
			}
			else {
				AtomicList.store(nullptr, std::memory_order_release);
			}
		}

		inline void Destory() {
			AtomicList.store(nullptr, std::memory_order_release);
		}

		inline Dlist& Load() {
			if (IsThread){
				Dlist* pList = AtomicList.load(std::memory_order_acquire);
				if (pList){
					List = *pList;
				}
				else{
					AtomicList.store(&List, std::memory_order_release);
				}			
			}
			return List;
		}

		inline void Store()
		{
			if (IsThread)
			{
				AtomicList.store(&List,std::memory_order_release);
			}
		}
	private:
		bool IsThread;
		std::atomic<Dlist*> AtomicList;
		Dlist                    List;
	};
}

#endif // !_XFDELEGATEARRAY_H_

