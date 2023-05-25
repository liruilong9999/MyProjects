#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

class EventBus
{
public:
    struct CallbackItem
    {
        std::function<void(int, void*, int)> callback;
        int                                  priority;
        void*                                rawObject;
    };

    struct EventCallbacksItem
    {
        std::vector<CallbackItem> callbacks;
        std::mutex                mutex; // 用于保护回调函数列表的互斥锁
    };

    struct EventQueueItem
    {
        int                       eventCode;
        void*                     data;
        int                       receiverId;
        std::vector<CallbackItem> callbacks;
    };

    struct StickyEvent
    {
        int   eventCode;
        void* data;
        int   receiverId;
    };

    void post(int eventCode, void* data, int receiverId);

    void subscribe(int eventCode, std::function<void(int, void*, int)> callback, int priority = 0, void* rawObject = nullptr);

    void unsubscribe(int eventCode, std::function<void(int, void*, int)> callback);

    void unsubscribeByRawObject(void* rawObject);

    void handlePost();

    void postStickyEvent(int eventCode, void* data, int receiverId);

    void handleStickyEvents();

private:
    std::unordered_map<int, EventCallbacksItem> eventCallbacks;
    std::queue<EventQueueItem>                  eventQueue;
    std::unordered_map<int, StickyEvent>        stickyEvents;
    std::mutex                                  eventQueueMutex; // 用于保护事件队列的互斥锁
    std::condition_variable                     eventQueueCV;    // 用于事件队列的条件变量，用于唤醒等待处理事件的线程

    void handleEvents(const EventQueueItem& eventQueueItem);
};

// 发布一个事件
void EventBus::post(int eventCode, void* data, int receiverId)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    auto                        eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem != eventCallbacks.end())
    {
        EventQueueItem eventQueueItem = {eventCode, data, receiverId, eventCallbacksItem->second.callbacks};
        eventQueue.push(eventQueueItem);
        eventQueueCV.notify_all(); // 唤醒等待处理事件的线程
    }
}

// 订阅一个事件
void EventBus::subscribe(int eventCode, std::function<void(int, void*, int)> callback, int priority, void* rawObject)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    CallbackItem                callbackItem = {callback, priority, rawObject};
    eventCallbacks[eventCode].callbacks.push_back(callbackItem);
    std::sort(eventCallbacks[eventCode].callbacks.begin(), eventCallbacks[eventCode].callbacks.end(), [](const CallbackItem& a, const CallbackItem& b)
              { return a.priority > b.priority; });
}

// 取消订阅一个事件
void EventBus::unsubscribe(int eventCode, std::function<void(int, void*, int)> callback)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    auto&                       callbacks = eventCallbacks[eventCode].callbacks;
    callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [&](const CallbackItem& item)
                                   { return item.callback.target_type() == callback.target_type(); }),
                    callbacks.end());
    if (callbacks.empty())
    {
        eventCallbacks.erase(eventCode);
    }
}

// 根据对象取消订阅事件
void EventBus::unsubscribeByRawObject(void* rawObject)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    for (auto& eventCallbacksItem : eventCallbacks)
    {
        std::lock_guard<std::mutex> lock(eventCallbacksItem.second.mutex);
        auto&                       callbacks = eventCallbacksItem.second.callbacks;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [&](const CallbackItem& item)
                                       { return item.rawObject == rawObject; }),
                        callbacks.end());
        if (callbacks.empty())
        {
            eventCallbacks.erase(eventCallbacksItem.first);
        }
    }
}

// 处理事件队列中的事件
void EventBus::handlePost()
{
    while (true)
    {
        EventQueueItem eventQueueItem;
        {
            std::unique_lock<std::mutex> lock(eventQueueMutex);
            eventQueueCV.wait(lock, [&]
                              { return !eventQueue.empty(); }); // 等待事件队列不为空
            eventQueueItem = eventQueue.front();
            eventQueue.pop();
        }
        handleEvents(eventQueueItem);
    }
}

// 处理事件队列中的事件项
void EventBus::handleEvents(const EventQueueItem& eventQueueItem)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventQueueItem.eventCode].mutex);
    for (const CallbackItem& callbackItem : eventQueueItem.callbacks)
    {
        callbackItem.callback(eventQueueItem.eventCode, eventQueueItem.data, eventQueueItem.receiverId);
    }
}

// 发布一个粘性事件
void EventBus::postStickyEvent(int eventCode, void* data, int receiverId)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    StickyEvent                 stickyEvent = {eventCode, data, receiverId};
    stickyEvents[eventCode]                 = stickyEvent;
    post(eventCode, data, receiverId);
}

// 处理粘性事件
void EventBus::handleStickyEvents()
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    for (auto& stickyEvent : stickyEvents)
    {
        handleEvents({stickyEvent.second.eventCode, stickyEvent.second.data, stickyEvent.second.receiverId, eventCallbacks[stickyEvent.second.eventCode].callbacks});
    }
}
