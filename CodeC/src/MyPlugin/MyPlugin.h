#ifndef MY_PLUGIN_H__
#define MY_PLUGIN_H__

#include <iostream>
#include <CPlusPlusReflection\OrginIPlugin.h>

struct MyPlugin : OrginIPlugin 
{
public:
    void Initialize() override;
};

#endif // !MY_PLUGIN_H__