#include "LDynamicLib.h"

HMODULE LDynamicLibrary::LoadDynamicLibrary(const std::string& LibPathName)
{
#ifdef _WIN32
    HINSTANCE handle = LoadLibrary(LibPathName.c_str()); // Windows下的动态库加载函数
#else
    void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY); // Linux下的动态库加载函数
#endif
    return handle;
}

HMODULE LDynamicLibrary::LoadConditionsDynamicLibrary(const std::string& LibPathName)
{
    return nullptr;
}

FARPROC LDynamicLibrary::GetFounctionByLibrary(HMODULE hModule, const std::string& funcName)
{
    return nullptr;
}

void LDynamicLibrary::FreeDynamicLibrary(HMODULE hModule)
{
}

void LDynamicLibrary::FreeAllLoadedDynamicLibrary()
{
}
