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
				std::
			}
#else
			static T instance;
			return &instance;
#endif
		}
	protected:
		Singleton() default;

	private:
		~Singleton() default;

	private:
		void Destory() {
#ifdef THREAD_SINGLETON

#endif
		}
	private:
		std::atomic<T*> _instance;
		std::mutex      _mutex;
	};
}



#endif //_XFSINGLETON_H_
