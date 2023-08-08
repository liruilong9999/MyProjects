#ifndef LBASECOMMON_H__
#define LBASECOMMON_H__

//该文件对所有的工具类进行整合
#ifdef LBASECOMMON_LIBRARY
#define LBASECOMMON_EXPORT __declspec(dllexport)
#else
#define LBASECOMMON_EXPORT __declspec(dllimport)
#endif

#endif // !LBASECOMMON_H__