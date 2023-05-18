#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "LSingleton.h"

class LBASECOMMON_EXPORT EventBus:public LSingleton<EventBus>
{
	friend class LSingleton<EventBus>;
public:
    struct CallbackItem
    {
		//用于存储回调函数的相关信息，包括回调函数的优先级、函数指针等。
        std::function<void(int, void*, int)> callback;  // 回调函数
        int                                  priority;  // 优先级
        void*                                rawObject; // 对象指针，用于根据对象取消订阅事件
    };

    struct EventCallbacksItem
    {
		//用于存储事件和对应的回调函数列表。
        std::vector<CallbackItem> callbacks; // 回调函数列表
    };

    struct EventQueueItem
    {
		//表示待处理的事件项，包含事件的代码、参数、订阅者列表等。
        int                       eventCode;  // 事件代码
        void*                     data;       // 参数
        int                       receiverId; // 接收者ID
        std::vector<CallbackItem> callbacks;  // 回调函数列表
    };

    struct StickyEvent
    {
		//用于存储粘性事件的信息，粘性事件是一种特殊的事件，它会一直保留在事件队列中，直到被处理。
        int   eventCode;  // 事件代码
        void* data;       // 参数
        int   receiverId; // 接收者ID
    };

    // 发布一个事件,将事件存储在事件队列中
    void post(int eventCode, void* data, int receiverId);

    // 订阅一个事件
    void subscribe(int eventCode, std::function<void(int, void*, int)> callback, int priority = 0, void* rawObject = nullptr);

    // 取消订阅一个事件
    void unsubscribe(int eventCode, std::function<void(int, void*, int)> callback);

    // 根据对象取消订阅事件
    void unsubscribeByRawObject(void* rawObject);

    // 处理事件队列中的事件
    void handlePost();

    // 发布一个粘性事件
    void postStickyEvent(int eventCode, void* data, int receiverId);

    // 处理粘性事件
    void handleStickyEvents();

private:
    std::unordered_map<int, EventCallbacksItem> eventCallbacks; // 存储事件和对应的回调函数列表
    std::vector<EventQueueItem>                 eventQueue;     // 存储待处理的事件队列
    std::unordered_map<int, StickyEvent>        stickyEvents;   // 存储粘性事件的信息

    // 处理事件队列中的事件项
    void handleEvents(const EventQueueItem& eventQueueItem);
};
