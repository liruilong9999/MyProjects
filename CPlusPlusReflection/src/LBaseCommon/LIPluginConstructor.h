#ifndef L_I_PLUGIN_H__
#define L_I_PLUGIN_H__

#include "LBaseCommon.h"
#include "LDeclBaseType.h"

// ×¢²á»ùÀà
class LBASECOMMON_EXPORT LIPluginConstructor
{
public:
    virtual ~LIPluginConstructor();
    virtual void Initialize() = 0;
};

DEF_CLASSSHARED_PTR(LIPluginConstructor);

// ²å¼þ×¢²áºê
#define DEF_PlUGINREGISTERCONSTRUCTIR(pluginName)       \
    class pluginName##Constructor : public LIPluginConstructor \
    {                                                   \
    public:                                             \
        virtual LIViewPlugin ConstructorPlugin()         \
        {\
            \
        }\
    };

#endif
