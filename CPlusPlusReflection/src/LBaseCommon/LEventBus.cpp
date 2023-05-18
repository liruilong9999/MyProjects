#include "LEventBus.h"

#include <algorithm>

// 发布一个事件
void EventBus::post(int eventCode, void* data, int receiverId)
{
    auto eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem != eventCallbacks.end())
    {
        EventQueueItem eventQueueItem = {eventCode, data, receiverId, eventCallbacksItem->second.callbacks};
        eventQueue.push_back(eventQueueItem);
    }
}

// 订阅一个事件
void EventBus::subscribe(int eventCode, std::function<void(int, void*, int)> callback, int priority, void* rawObject)
{
    auto eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem == eventCallbacks.end())
    {
        eventCallbacksItem = eventCallbacks.emplace(eventCode, EventCallbacksItem()).first;
    }
    CallbackItem callbackItem = {callback, priority, rawObject};
    eventCallbacksItem->second.callbacks.push_back(callbackItem);
    std::sort(eventCallbacksItem->second.callbacks.begin(), eventCallbacksItem->second.callbacks.end(), [](const CallbackItem& a, const CallbackItem& b)
              { return a.priority > b.priority; }); // 按优先级排序
}

// 取消订阅一个事件
void EventBus::unsubscribe(int eventCode, std::function<void(int, void*, int)> callback)
{
    auto eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem != eventCallbacks.end())
    {
        eventCallbacksItem->second.callbacks.erase(std::remove_if(eventCallbacksItem->second.callbacks.begin(), eventCallbacksItem->second.callbacks.end(), [&](const CallbackItem& item)
                                                                  { return item.callback.target_type() == callback.target_type(); }),
                                                   eventCallbacksItem->second.callbacks.end());
        if (eventCallbacksItem->second.callbacks.empty())
        {
            eventCallbacks.erase(eventCode);
        }
    }
}

// 根据对象取消订阅事件
void EventBus::unsubscribeByRawObject(void* rawObject)
{
    for (auto& eventCallbacksItem : eventCallbacks)
    {
        eventCallbacksItem.second.callbacks.erase(std::remove_if(eventCallbacksItem.second.callbacks.begin(), eventCallbacksItem.second.callbacks.end(), [&](const CallbackItem& item)
                                                                 { return item.rawObject == rawObject; }),
                                                  eventCallbacksItem.second.callbacks.end());
        if (eventCallbacksItem.second.callbacks.empty())
        {
            eventCallbacks.erase(eventCallbacksItem.first);
        }
    }
}

// 处理事件队列中的事件
void EventBus::handlePost()
{
    while (!eventQueue.empty())
    {
        EventQueueItem eventQueueItem = eventQueue.front();
        eventQueue.erase(eventQueue.begin());
        handleEvents(eventQueueItem);
    }
}

// 处理事件队列中的事件项
void EventBus::handleEvents(const EventQueueItem& eventQueueItem)
{
    for (const CallbackItem& callbackItem : eventQueueItem.callbacks)
    {
        callbackItem.callback(eventQueueItem.eventCode, eventQueueItem.data, eventQueueItem.receiverId);
    }
}

// 发布一个粘性事件
void EventBus::postStickyEvent(int eventCode, void* data, int receiverId)
{
    StickyEvent stickyEvent = {eventCode, data, receiverId};
    stickyEvents[eventCode] = stickyEvent;
    post(eventCode, data, receiverId);
}

// 处理粘性事件
void EventBus::handleStickyEvents()
{
    for (auto& stickyEvent : stickyEvents)
    {
        handleEvents({stickyEvent.second.eventCode, stickyEvent.second.data, stickyEvent.second.receiverId, eventCallbacks[stickyEvent.second.eventCode].callbacks});
    }
}
