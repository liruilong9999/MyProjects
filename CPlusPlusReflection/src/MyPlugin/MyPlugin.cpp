#include "MyPlugin.h"
void MyPlugin::Initialize() 
{
    std::cout << "插件加载成功，执行插件内容" << std::endl;
}

// 插件创建函数
extern "C" __declspec(dllexport) OrginIPlugin * createPlugin()
{
    return new MyPlugin();  // 替换为你的插件类的实例化
}