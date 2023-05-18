#ifndef LSINGLETON_H__
#define LSINGLETON_H__

#include "LDeclBaseType.h"

//单例基类，继承该类便为单例类
//当前写法，子类必须声明友元，尝试改为奇异递归模板模式
template<typename T>
class LBASECOMMON_EXPORT LSingleton
{
public:
	static T& GetInstance() {
		static T instance;
		return instance;
	}

protected:
	LSingleton() = default;
	~LSingleton() = default;
    LSingleton(const LSingleton&)            = delete;
    LSingleton& operator=(const LSingleton&) = delete;
    LSingleton(LSingleton&&)                 = delete;
    LSingleton& operator=(LSingleton&&)      = delete;
};

//注册友元宏
#define SINGLETON_DECLARE(className) \
friend class LSingleton<className>;



/*
//子类实现demo
class mysingleton :public LSingleton<mysingleton>
{
	SINGLETON_DECLARE(mysingleton);

	mysingleton() = default;
	~mysingleton() = default;

public:
	void print() 
	{
		//xxx;
	}
};

//调用
void use()
{
	mysingleton::getInstance().print();
}
*/

#endif // !LSINGLETON_H__