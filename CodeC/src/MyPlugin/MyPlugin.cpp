#include "MyPlugin.h"
void MyPlugin::Initialize() 
{
    std::cout << "������سɹ���ִ�в������" << std::endl;
}

// �����������
extern "C" __declspec(dllexport) OrginIPlugin * createPlugin()
{
    return new MyPlugin();  // �滻Ϊ��Ĳ�����ʵ����
}