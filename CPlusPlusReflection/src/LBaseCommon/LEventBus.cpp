#include "LEventBus.h"

#include <algorithm>

// ����һ���¼�
void EventBus::post(int eventCode, void* data, int receiverId)
{
    auto eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem != eventCallbacks.end())
    {
        EventQueueItem eventQueueItem = {eventCode, data, receiverId, eventCallbacksItem->second.callbacks};
        eventQueue.push_back(eventQueueItem);
    }
}

// ����һ���¼�
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
              { return a.priority > b.priority; }); // �����ȼ�����
}

// ȡ������һ���¼�
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

// ���ݶ���ȡ�������¼�
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

// �����¼������е��¼�
void EventBus::handlePost()
{
    while (!eventQueue.empty())
    {
        EventQueueItem eventQueueItem = eventQueue.front();
        eventQueue.erase(eventQueue.begin());
        handleEvents(eventQueueItem);
    }
}

// �����¼������е��¼���
void EventBus::handleEvents(const EventQueueItem& eventQueueItem)
{
    for (const CallbackItem& callbackItem : eventQueueItem.callbacks)
    {
        callbackItem.callback(eventQueueItem.eventCode, eventQueueItem.data, eventQueueItem.receiverId);
    }
}

// ����һ��ճ���¼�
void EventBus::postStickyEvent(int eventCode, void* data, int receiverId)
{
    StickyEvent stickyEvent = {eventCode, data, receiverId};
    stickyEvents[eventCode] = stickyEvent;
    post(eventCode, data, receiverId);
}

// ����ճ���¼�
void EventBus::handleStickyEvents()
{
    for (auto& stickyEvent : stickyEvents)
    {
        handleEvents({stickyEvent.second.eventCode, stickyEvent.second.data, stickyEvent.second.receiverId, eventCallbacks[stickyEvent.second.eventCode].callbacks});
    }
}
