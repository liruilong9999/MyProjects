#ifndef ORGIN_I_PLUGIN_H__
#define ORGIN_I_PLUGIN_H__

#ifdef CPLUSPLUSREFLECTION_LIBRARY
#define CPLUSPLUSREFLECTION_EXPORT __declspec(dllexport)
#else
#define CPLUSPLUSREFLECTION_EXPORT __declspec(dllimport)
#endif


class CPLUSPLUSREFLECTION_EXPORT OrginIPlugin
{
public:
    virtual ~OrginIPlugin();
    virtual void Initialize() = 0;
};

#endif


