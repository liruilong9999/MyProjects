#ifndef LAPPCONFIG_H__
#define LAPPCONFIG_H__
#include "LSingleton.h"

struct LAppConfigPrivate;
DEF_STRUCTHARED_PTR(LAppConfigPrivate);

class LBASECOMMON_EXPORT LAppConfig: public LSingleton<LAppConfig>
{
	SINGLETON_DECLARE(LAppConfig);
public:
	LAppConfig();
	~LAppConfig();

	//设置配置文件路径
	void SetPath(std::string path);
	//获取配置文件路径
	std::string GetPath();
	//获取插件列表
    template <class T>
	T GetPluginList();
	//获取模块列表
    template <class T>
	T GetModuleList();
	//获取配置项
    template <class T>
	T GetAppConfig(std::string configName, T value);
	//设置配置项
    template <class T>
	void SetAppConfig(std::string configName , T value);

private:
	LAppConfigPrivate* _P;
};

#endif