#include "LDynamicLib.h"


LPCWSTR CharToLPCWSTR(const char * str)
{
    int       size   = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    wchar_t * buffer = new wchar_t[size];
    MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, size);
    LPCWSTR result = buffer;
    delete[] buffer;
    return result;
}

HMODULE LDynamicLibrary::LoadDynamicLibrary(const std::string& LibPathName)
{
#ifdef _WIN32
    HINSTANCE handle = LoadLibrary(CharToLPCWSTR(LibPathName.c_str())); // Windows下的动态库加载函数
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
#ifdef _WIN32
    FreeLibrary(hModule); // Windows下的动态库关闭函数
#else
    dlclose(handle);                                      // Linux下的动态库关闭函数
#endif
}

void LDynamicLibrary::FreeAllLoadedDynamicLibrary()
{
    // 关闭动态链接库
    for (void* handle : params::handles)
    {
#ifdef _WIN32
        FreeLibrary((HMODULE)handle); // Windows下的动态库关闭函数
#else
        dlclose(handle);                                  // Linux下的动态库关闭函数
#endif
    }
}
