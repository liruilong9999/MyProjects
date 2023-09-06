#ifndef LDYNAMICLIB_H__
#define LDYNAMICLIB_H__
// 该文件用于加载动态链接库
#include "LDeclBaseType.h"

#ifdef _WIN32
#include <windows.h> // Windows下的动态链接库头文件
#else
#include <dlfcn.h> // Linux下的动态链接库头文件
#endif
namespace LDynamicLibrary
{
    namespace params
    {
        std::vector<void*> handles;    
    };
    // 加载动态库返回类型HMODULE的值为空，表示加载失败

    // 加载动态链接库
    // LibPathName 动态库的路径+动态库名称
    HMODULE LoadDynamicLibrary(const std::string& LibPathName);

    // 根据不同的编译条件，版本和系统类型加载合适的动态链接库
    // LibPathName 动态库的路径+动态库名称
    HMODULE LoadConditionsDynamicLibrary(const std::string& LibPathName);

    // 从指定动态链接库中获取指定函数
    // hModule 动态库对象 ； funcName 函数名称
    FARPROC GetFounctionByLibrary(HMODULE hModule, const std::string& funcName);

    // 释放指定动态链接库
    // hModule 动态库对象
    void FreeDynamicLibrary(HMODULE hModule);

    // 释放已加载的全部动态库
    void FreeAllLoadedDynamicLibrary();
}; // namespace LDynamicLibrary

#endif
