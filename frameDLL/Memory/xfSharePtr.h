#ifndef _XFSharedPtr_H_
#define _XFSharedPtr_H_

#include <atomic>

namespace XFRAME
{
	template<typename T>
	class SharedPtr
	{
		class SharedCounter
		{
		public:
			SharedCounter(int n)
				: Count(n) { }
			~SharedCounter() = default;

			void Ref() { Count++; }

			void UnRef() { Count--; }

			int GetCount() { return Count; }
		private:
			int Count;
		};
	public:
		SharedPtr() 
			: Data(nullptr),
			SCounter(nullptr){
		}

		explicit SharedPtr(T&& d)
			: Data(new T{ std::move(d) }),
			SCounter(new SharedCounter(1)){

		}

		explicit SharedPtr(const T& d)
			: Data(new T(d)),
			SCounter(new SharedCounter(1)) {

		}

		explicit SharedPtr(T* d)
			:Data(d),
			SCounter(new SharedCounter(1)) {

		}

		SharedPtr(const SharedPtr<T>& other) {
			if (other.SCounter){
				other.SCounter->Ref();
			}
			ThreadSafe = other.ThreadSafe;
			SCounter = other.SCounter;
			Data = other.Data;
			SetThreadSafe(ThreadSafe);
		}

		SharedPtr(SharedPtr<T>&& other)
			: Data(std::move(other.Data)),
			SCounter(other.SCounter){
			this->ThreadSafe = other.ThreadSafe;
			other.Data = nullptr;
			other.Data = nullptr;
			SetThreadSafe(ThreadSafe);
		}

		~SharedPtr() {
			Release();
		}
	public:
		T& operator*(){
			return *GetData();
		}

		T* operator->() {
			return GetData();
		}

		SharedPtr& operator=(const SharedPtr<T>& other) {
			if (other.SCounter){
				other.SCounter->Ref();
			}
			Release();
			this->Data = other.Data;
			this->SCounter = other.SCounter;
			this->ThreadSafe = other.ThreadSafe;
			SetThreadSafe(ThreadSafe);
			return *this;
		}

		SharedPtr& operator=(T* d) {
			if (d){
				throw"Error : Unable to assign value";
			}
			else {
				Release();
			}
			return *this;
		}

		bool operator==(T* d) {
			return GetData() == d;
		}

		bool operator==(const SharedPtr<T>& other) {
			if (Data == other.Data &&
				SCounter == other.SCounter &&
				ThreadSafe == other.ThreadSafe){
				return true;
			}
			return false;
		}

	public:
		void SetThreadSafe(bool ts) {
			ThreadSafe = ts;
			if (ThreadSafe){
				AtomicData.store(Data, std::memory_order_release);
			}
			else {
				AtomicData.store(nullptr, std::memory_order_release);
			}
		}
	private:
		void Release() {
			if (this->SCounter){
				this->SCounter->UnRef();
			}

			if (this->SCounter && 
				this->SCounter->GetCount() == 0) {
				delete this->SCounter;
				delete this->Data;
				this->SCounter = nullptr;
				this->Data = nullptr;
			}
			else {
				this->SCounter = nullptr;
				this->Data = nullptr;
			}

			if (ThreadSafe) {
				AtomicData.store(nullptr, std::memory_order_release);
			}
		}

		T* GetData() {
			T* rData = nullptr;
			if (ThreadSafe) {
				rData = AtomicData.load(std::memory_order_acquire);
				Data = rData;
			}
			else {
				rData = this->Data;
			}
			return rData;
		}

	private:
		mutable SharedCounter*       SCounter;
		T*                           Data;
		std::atomic<T*>              AtomicData;
		bool                         ThreadSafe = false;
	};
}

#endif //!_XFSharedPtr_H_
