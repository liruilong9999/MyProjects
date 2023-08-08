#ifndef LVIEWPLUGINFACTORY_H__
#define LVIEWPLUGINFACTORY_H__

#include "LDeclBaseType.h"
#include "LSingleton.h"

//插件工厂类
class LViewPluginFactory : public LSingleton<LViewPluginFactory>
{
    SINGLETON_DECLARE(LViewPluginFactory);
public:
    //加载插件
    void LoadPlugins();
    //注册插件
    void RegisterViewPluginConstructor(int );
    //根据插件名称创建插件

    //根据插件名称获取插件

    //按照插件名称注册插件

    //


};



#endif // LVIEWPLUGINFACTORY_H__