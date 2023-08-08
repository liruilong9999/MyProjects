#include <iostream>
#include <vector>
#include <string>
// #include <dlfcn.h> // Linux下的动态链接库头文件
#include <windows.h> // Windows下的动态链接库头文件

#include <CPlusPlusReflection\OrginIPlugin.h>

typedef OrginIPlugin *(*CreatePluginFunc)(); // 插件创建函数指针类型

// 将 const char* 转换为 LPCWSTR
LPCWSTR ConvertToLPCWSTR(const char *str)
{
    // 计算宽字符所需的缓冲区大小（以字节为单位）
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

    // 创建宽字符缓冲区
    wchar_t *buffer = new wchar_t[size];

    // 进行字符编码转换
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, size);

    // 返回转换后的 LPCWSTR
    return buffer;
}

int main()
{
    std::vector<std::string> pluginNames = {"MyPlugind"};                                                                     // 从配置文件获取插件名称列表
    std::vector<std::string> pluginPaths = {"D:\\MyProjects\\CPlusPlusReflection\\build\\bin\\MyPlugind.dll"}; // 从配置文件获取插件路径列表

    std::vector<OrginIPlugin *> plugins;

    std::vector<void*> handles;

    for (size_t i = 0; i < pluginNames.size(); ++i)
    {
        std::string pluginName = pluginNames[i];
        std::string pluginPath = pluginPaths[i];

        // 动态加载插件库
        // void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY); // Linux下的动态库加载函数
        LPCWSTR wstr = ConvertToLPCWSTR(pluginPath.c_str());
        //HINSTANCE handle = LoadLibrary(wstr); // Windows下的动态库加载函数
        HINSTANCE handle = LoadLibrary(pluginPath.c_str()); // Windows下的动态库加载函数

        delete[] wstr;

        if (!handle)
        {
            std::cerr << "无法加载插件：" << pluginName << std::endl;
            continue;
        }

        // 获取插件创建函数地址
        // CreatePluginFunc createPluginFunc = (CreatePluginFunc)dlsym(handle, "createPlugin"); // Linux下的函数获取方式
        CreatePluginFunc createPluginFunc = (CreatePluginFunc)GetProcAddress(handle, "createPlugin"); // Windows下的函数获取方式

        if (!createPluginFunc)
        {
            std::cerr << "无法获取插件创建函数：" << pluginName << std::endl;
            continue;
        }

        // 创建插件实例
        OrginIPlugin *plugin = createPluginFunc();

        if (!plugin)
        {
            std::cerr << "无法创建插件实例：" << pluginName << std::endl;
            continue;
        }

        // 初始化插件
        plugin->Initialize();

        // 将插件添加到列表中
        plugins.push_back(plugin);

        handles.push_back(handle);
    }
    // 释放插件资源
    for (OrginIPlugin *plugin : plugins)
    {
        delete plugin;
    }

    // 关闭动态链接库
    for (void *handle : handles)
    {
        // dlclose(handle); // Linux下的动态库关闭函数
        FreeLibrary((HMODULE)handle); // Windows下的动态库关闭函数
    }
    return 0;
}