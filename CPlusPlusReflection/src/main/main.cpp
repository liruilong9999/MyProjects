#include <iostream>
#include <vector>
#include <string>
// #include <dlfcn.h> // Linux�µĶ�̬���ӿ�ͷ�ļ�
#include <windows.h> // Windows�µĶ�̬���ӿ�ͷ�ļ�

#include <CPlusPlusReflection\OrginIPlugin.h>

typedef OrginIPlugin *(*CreatePluginFunc)(); // �����������ָ������

// �� const char* ת��Ϊ LPCWSTR
LPCWSTR ConvertToLPCWSTR(const char *str)
{
    // ������ַ�����Ļ�������С�����ֽ�Ϊ��λ��
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

    // �������ַ�������
    wchar_t *buffer = new wchar_t[size];

    // �����ַ�����ת��
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, size);

    // ����ת����� LPCWSTR
    return buffer;
}

int main()
{
    std::vector<std::string> pluginNames = {"MyPlugind"};                                                                     // �������ļ���ȡ��������б�
    std::vector<std::string> pluginPaths = {"D:\\MyProjects\\CPlusPlusReflection\\build\\bin\\MyPlugind.dll"}; // �������ļ���ȡ���·���б�

    std::vector<OrginIPlugin *> plugins;

    std::vector<void*> handles;

    for (size_t i = 0; i < pluginNames.size(); ++i)
    {
        std::string pluginName = pluginNames[i];
        std::string pluginPath = pluginPaths[i];

        // ��̬���ز����
        // void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY); // Linux�µĶ�̬����غ���
        LPCWSTR wstr = ConvertToLPCWSTR(pluginPath.c_str());
        //HINSTANCE handle = LoadLibrary(wstr); // Windows�µĶ�̬����غ���
        HINSTANCE handle = LoadLibrary(pluginPath.c_str()); // Windows�µĶ�̬����غ���

        delete[] wstr;

        if (!handle)
        {
            std::cerr << "�޷����ز����" << pluginName << std::endl;
            continue;
        }

        // ��ȡ�������������ַ
        // CreatePluginFunc createPluginFunc = (CreatePluginFunc)dlsym(handle, "createPlugin"); // Linux�µĺ�����ȡ��ʽ
        CreatePluginFunc createPluginFunc = (CreatePluginFunc)GetProcAddress(handle, "createPlugin"); // Windows�µĺ�����ȡ��ʽ

        if (!createPluginFunc)
        {
            std::cerr << "�޷���ȡ�������������" << pluginName << std::endl;
            continue;
        }

        // �������ʵ��
        OrginIPlugin *plugin = createPluginFunc();

        if (!plugin)
        {
            std::cerr << "�޷��������ʵ����" << pluginName << std::endl;
            continue;
        }

        // ��ʼ�����
        plugin->Initialize();

        // �������ӵ��б���
        plugins.push_back(plugin);

        handles.push_back(handle);
    }
    // �ͷŲ����Դ
    for (OrginIPlugin *plugin : plugins)
    {
        delete plugin;
    }

    // �رն�̬���ӿ�
    for (void *handle : handles)
    {
        // dlclose(handle); // Linux�µĶ�̬��رպ���
        FreeLibrary((HMODULE)handle); // Windows�µĶ�̬��رպ���
    }
    return 0;
}