#ifndef LDECLBASETYPE_H__
#define LDECLBASETYPE_H__

#include "LBaseCommon.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

//封装智能指针，使其变为宏方便快速调用
#define DEF_CLASSSHARED_PTR(className) \
	class className;\
	using className##Ptr = std::shared_ptr<className>;
#define DEF_STRUCTHARED_PTR(structName) \
	struct structName;\
	using structName##Ptr = std::shared_ptr<structName>;















#endif 

