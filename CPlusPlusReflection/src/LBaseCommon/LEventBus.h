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
        std::mutex                mutex; // ���ڱ����ص������б�Ļ�����
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
    std::mutex                                  eventQueueMutex; // ���ڱ����¼����еĻ�����
    std::condition_variable                     eventQueueCV;    // �����¼����е��������������ڻ��ѵȴ������¼����߳�

    void handleEvents(const EventQueueItem& eventQueueItem);
};

// ����һ���¼�
void EventBus::post(int eventCode, void* data, int receiverId)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    auto                        eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem != eventCallbacks.end())
    {
        EventQueueItem eventQueueItem = {eventCode, data, receiverId, eventCallbacksItem->second.callbacks};
        eventQueue.push(eventQueueItem);
        eventQueueCV.notify_all(); // ���ѵȴ������¼����߳�
    }
}

// ����һ���¼�
void EventBus::subscribe(int eventCode, std::function<void(int, void*, int)> callback, int priority, void* rawObject)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    CallbackItem                callbackItem = {callback, priority, rawObject};
    eventCallbacks[eventCode].callbacks.push_back(callbackItem);
    std::sort(eventCallbacks[eventCode].callbacks.begin(), eventCallbacks[eventCode].callbacks.end(), [](const CallbackItem& a, const CallbackItem& b)
              { return a.priority > b.priority; });
}

// ȡ������һ���¼�
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

// ���ݶ���ȡ�������¼�
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

// �����¼������е��¼�
void EventBus::handlePost()
{
    while (true)
    {
        EventQueueItem eventQueueItem;
        {
            std::unique_lock<std::mutex> lock(eventQueueMutex);
            eventQueueCV.wait(lock, [&]
                              { return !eventQueue.empty(); }); // �ȴ��¼����в�Ϊ��
            eventQueueItem = eventQueue.front();
            eventQueue.pop();
        }
        handleEvents(eventQueueItem);
    }
}

// �����¼������е��¼���
void EventBus::handleEvents(const EventQueueItem& eventQueueItem)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventQueueItem.eventCode].mutex);
    for (const CallbackItem& callbackItem : eventQueueItem.callbacks)
    {
        callbackItem.callback(eventQueueItem.eventCode, eventQueueItem.data, eventQueueItem.receiverId);
    }
}

// ����һ��ճ���¼�
void EventBus::postStickyEvent(int eventCode, void* data, int receiverId)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    StickyEvent                 stickyEvent = {eventCode, data, receiverId};
    stickyEvents[eventCode]                 = stickyEvent;
    post(eventCode, data, receiverId);
}

// ����ճ���¼�
void EventBus::handleStickyEvents()
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    for (auto& stickyEvent : stickyEvents)
    {
        handleEvents({stickyEvent.second.eventCode, stickyEvent.second.data, stickyEvent.second.receiverId, eventCallbacks[stickyEvent.second.eventCode].callbacks});
    }
}
