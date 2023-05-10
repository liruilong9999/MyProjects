#include "LDynamicLib.h"

HMODULE LDynamicLibrary::LoadDynamicLibrary(const std::string& LibPathName)
{
#ifdef _WIN32
    HINSTANCE handle = LoadLibrary(LibPathName.c_str()); // Windows�µĶ�̬����غ���
#else
    void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY); // Linux�µĶ�̬����غ���
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
