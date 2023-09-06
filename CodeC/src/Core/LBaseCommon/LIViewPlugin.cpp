#include "LIViewPlugin.h"


struct LIViewPluginParam
{
	LIViewPluginParam()
	{

	}
    std::string _PluginName;
    std::string _PluginDiscripe;
    std::string _PluginClassName;
};

LIViewPlugin::LIViewPlugin()
	:_P(new LIViewPluginParam())
{

}


LIViewPlugin::~LIViewPlugin()
{
	if (_P)
	{
		delete _P;
		_P = nullptr;
	}
}

void LIViewPlugin::Initialize(LIViewPluginParamPtr Param)
{

}

void LIViewPlugin::Cleanuo()
{

}

std::string LIViewPlugin::GetName()
{
	return "";
}

std::string LIViewPlugin::GetDiscripe()
{
	return "";
}

LIViewPluginParam LIViewPlugin::GetPluginParam()
{
	return LIViewPluginParam();
}

const std::string LIViewPlugin::GetPluginClassName()
{
	return "";
}

void LIViewPlugin::UsePlugin(bool IsUse)
{

}

void LIViewPlugin::SetPluginClassName(const std::string& className)
{
    _P->_PluginClassName = className;
}
