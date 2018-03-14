#ifndef __SINGLETON_HEARDER_H__
#define __SINGLETON_HEARDER_H__
#include<boost/thread.hpp>
/**
 * 功能：单例模式的基类
 * 说明：需要集成使用
 */
template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		if(NULL==_single_instance)
		{
			static boost::mutex locker;
			std::lock_guard<std::mutex> g(locker);

			_single_instance=new(std::nothrow) T();
		}
	
		return *_single_instance;
	}

protected:
	Singleton();
	~Singleton();

	Singleton(const Singleton &other);
	T& operator==(const Singleton &rhs);
private:
	static T* _single_instance;
};

#endif
