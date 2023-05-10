#include "LAppConfig.h"

#include <mutex>
#include <shared_mutex>
#include <thread>

std::shared_mutex rwPathMutex;
int shared_data = 0;

struct LAppConfigPrivate
{
	LAppConfigPrivate()
		:Path("D : \\Code\\GitCode\\MyProjects\\CPlusPlusReflection\\src\\config\\LAppConfig.xml")
	{

	}

	std::string Path;
};

LAppConfig::~LAppConfig()
{
	if (_P)
	{
		delete _P;
		_P = nullptr;
	}
}

LAppConfig::LAppConfig()
	:_P(new LAppConfigPrivate)
{

}

template <class T>
void LAppConfig::SetAppConfig(std::string configName, T value)
{

}

template <class T>
T LAppConfig::GetAppConfig(std::string configName, T value)
{

}

template <class T>
T LAppConfig::GetModuleList()
{

}

template <class T>
T LAppConfig::GetPluginList()
{

}
std::string LAppConfig::GetPath()
{
	std::shared_lock<std::shared_mutex> lock(rwPathMutex);
	return _P->Path;
}

void LAppConfig::SetPath(std::string path)
{
	std::shared_lock<std::shared_mutex> lock(rwPathMutex);
    _P->Path = path;
}

