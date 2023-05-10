#ifndef LIVIEWPLUGIN_H__
#define LIVIEWPLUGIN_H__

#include "LSingleton.h"

struct LIViewPluginParam;
DEF_STRUCTHARED_PTR(LIViewPluginParam);


class LBASECOMMON_EXPORT LIViewPlugin//:public LSingleton<LIViewPlugin>
{
	//SINGLETON_DECLARE(LIViewPlugin);
public:
	LIViewPlugin();
	virtual ~LIViewPlugin();
	//初始化插件
	virtual void Initialize(LIViewPluginParamPtr Param);
	//清理插件
	virtual void Cleanuo();
	//获取插件名称
	virtual std::string GetName();
	//获取插件描述
	virtual std::string GetDiscripe();
	//获取插件参数
	LIViewPluginParam GetPluginParam();
	//获取插件类名
	const std::string GetPluginClassName();
	//设置插件类名
    void SetPluginClassName(const std::string & className);
	//启用插件/禁用插件
	virtual void UsePlugin(bool IsUse);
	
private:
	LIViewPluginParam* _P;
};

DEF_CLASSSHARED_PTR(LIViewPlugin);

#endif //LIVIEWPLUGIN_H__

