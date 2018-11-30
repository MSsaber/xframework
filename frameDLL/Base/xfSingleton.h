#ifndef _XFSINGLETON_H_
#define _XFSINGLETON_H_
/*
	singleton
*/
#include <atomic>
#include <mutex>

namespace XFRAME
{
	template<typename T>
	class Singleton
	{
	public:
		static T* Instance() {
#ifdef THREAD_SINGLETON
			{
				T* pinstance = _instance.load(std::memory_order_acquire);
				if (!pinstance)
				{
					pinstance = _instance.load(std::memory_order_relaxed);
					if (!pinstance)
					{
						std::lock_guard<std::mutex> mutex_lock(_mutex);
						pinstance = new T;
						_instance.store(pinstance, std::memory_order_release);
						std::atexit(&Singleton<T>::Destory());
					}
				}
			}
#else
			static T instance;
			return &instance;
#endif
		}
	protected:
		Singleton() = default;

	private:
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator =(const Singleton& other) = delete;
		Singleton&& operator =(Singleton&& other) = delete;

#ifdef THREAD_SINGLETON
	private:

		static void Destory() {
			T* pinstance = _instance.load(std::memory_order_acquire);
			if (pinstance)
			{
				_instance.store(nullptr, std::memory_order_release);
				delete pinstance;
				pinstance = nullptr;
			}
		}

		static std::atomic<T*> _instance;
		static std::mutex      _mutex;
#endif
	};
#ifdef THREAD_SINGLETON 
	template<typename T> std::atomic<T*> Singleton<T>::_instance;
	template<typename T> std::mutex      Singleton<T>::_mutex;
#endif
}

#endif //_XFSINGLETON_H_
