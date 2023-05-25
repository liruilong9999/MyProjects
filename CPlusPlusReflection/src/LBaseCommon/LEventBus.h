#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
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
        std::function<void(int, std::shared_ptr<void>, int)> callback;
        int                                                  priority;
        std::weak_ptr<void>                                  rawObject;

        bool operator==(const CallbackItem& other) const
        {
            return callback.target_type() == other.callback.target_type() &&
                   rawObject.lock() == other.rawObject.lock();
        }
    };

    struct EventCallbacksItem
    {
        std::vector<CallbackItem> callbacks;
        std::mutex                mutex;
    };

    struct EventQueueItem
    {
        int                       eventCode;
        std::shared_ptr<void>     data;
        int                       receiverId;
        std::vector<CallbackItem> callbacks;
    };

    struct StickyEvent
    {
        int                   eventCode;
        std::shared_ptr<void> data;
        int                   receiverId;
    };

    EventBus();
    ~EventBus();

    void post(int eventCode, std::shared_ptr<void> data, int receiverId);
    void subscribe(int eventCode, std::function<void(int, std::shared_ptr<void>, int)> callback, int priority = 0, std::shared_ptr<void> rawObject = nullptr);
    void unsubscribe(int eventCode, std::function<void(int, std::shared_ptr<void>, int)> callback);
    void unsubscribeByRawObject(std::shared_ptr<void> rawObject);
    void stop();

private:
    std::unordered_map<int, EventCallbacksItem> eventCallbacks;
    std::queue<EventQueueItem>                  eventQueue;
    std::unordered_map<int, StickyEvent>        stickyEvents;
    std::mutex                                  eventQueueMutex;
    std::condition_variable                     eventQueueCV;
    std::mutex                                  stickyEventsMutex;
    bool                                        running;
    std::thread                                 eventThread;

    void handlePost();
    void handleEvents(const EventQueueItem& eventQueueItem);
    void postStickyEvent(int eventCode, std::shared_ptr<void> data, int receiverId);
    void handleStickyEvents();
    void removeExpiredCallbacks();

    // 新增辅助函数
    void executeCallback(const CallbackItem& callbackItem, int eventCode, std::shared_ptr<void> data, int receiverId);
};

EventBus::EventBus()
    : running(true)
{
    eventThread = std::thread(&EventBus::handlePost, this);
}

EventBus::~EventBus()
{
    stop();
}

void EventBus::post(int eventCode, std::shared_ptr<void> data, int receiverId)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    auto                        eventCallbacksItem = eventCallbacks.find(eventCode);
    if (eventCallbacksItem != eventCallbacks.end())
    {
        EventQueueItem eventQueueItem = {eventCode, data, receiverId, eventCallbacksItem->second.callbacks};
        eventQueue.push(eventQueueItem);
        eventQueueCV.notify_all();
    }
}

void EventBus::subscribe(int eventCode, std::function<void(int, std::shared_ptr<void>, int)> callback, int priority, std::shared_ptr<void> rawObject)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    CallbackItem                callbackItem = {callback, priority, rawObject};
    eventCallbacks[eventCode].callbacks.push_back(callbackItem);
    std::sort(eventCallbacks[eventCode].callbacks.begin(), eventCallbacks[eventCode].callbacks.end(), [](const CallbackItem& a, const CallbackItem& b)
              { return a.priority > b.priority; });
}

void EventBus::unsubscribe(int eventCode, std::function<void(int, std::shared_ptr<void>, int)> callback)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    auto&                       callbacks = eventCallbacks[eventCode].callbacks;
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    if (callbacks.empty())
    {
        eventCallbacks.erase(eventCode);
    }
}

void EventBus::unsubscribeByRawObject(std::shared_ptr<void> rawObject)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    for (auto& eventCallbacksItem : eventCallbacks)
    {
        std::lock_guard<std::mutex> lock(eventCallbacksItem.second.mutex);
        auto&                       callbacks = eventCallbacksItem.second.callbacks;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [&](const CallbackItem& item)
                                       { return item.rawObject.lock() == rawObject; }),
                        callbacks.end());
        if (callbacks.empty())
        {
            eventCallbacks.erase(eventCallbacksItem.first);
        }
    }
}

void EventBus::handlePost()
{
    while (running)
    {
        EventQueueItem eventQueueItem;
        {
            std::unique_lock<std::mutex> lock(eventQueueMutex);
            eventQueueCV.wait(lock, [&]
                              { return !eventQueue.empty() || !running; });
            if (!running)
                return;
            eventQueueItem = eventQueue.front();
            eventQueue.pop();
        }
        handleEvents(eventQueueItem);
        removeExpiredCallbacks(); // 移除过期的回调函数
    }
}

void EventBus::handleEvents(const EventQueueItem& eventQueueItem)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventQueueItem.eventCode].mutex);
    for (const CallbackItem& callbackItem : eventQueueItem.callbacks)
    {
        auto rawObject = callbackItem.rawObject.lock();
        if (rawObject)
        {
            executeCallback(callbackItem, eventQueueItem.eventCode, eventQueueItem.data, eventQueueItem.receiverId);
        }
    }
}

void EventBus::postStickyEvent(int eventCode, std::shared_ptr<void> data, int receiverId)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    std::lock_guard<std::mutex> stickyLock(stickyEventsMutex);
    StickyEvent                 stickyEvent = {eventCode, data, receiverId};
    stickyEvents[eventCode]                 = stickyEvent;
    post(eventCode, data, receiverId);
}

void EventBus::handleStickyEvents()
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    std::lock_guard<std::mutex> stickyLock(stickyEventsMutex);
    for (const auto& stickyEvent : stickyEvents)
    {
        handleEvents({stickyEvent.second.eventCode, stickyEvent.second.data, stickyEvent.second.receiverId, eventCallbacks[stickyEvent.second.eventCode].callbacks});
    }
}

void EventBus::stop()
{
    {
        std::lock_guard<std::mutex> lock(eventQueueMutex);
        running = false;
        eventQueueCV.notify_all();
    }
    if (eventThread.joinable())
    {
        eventThread.join();
    }
}

void EventBus::removeExpiredCallbacks()
{
    for (auto& eventCallbacksItem : eventCallbacks)
    {
        std::lock_guard<std::mutex> lock(eventCallbacksItem.second.mutex);
        auto&                       callbacks = eventCallbacksItem.second.callbacks;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [](const CallbackItem& item)
                                       { return item.rawObject.expired(); }),
                        callbacks.end());
        if (callbacks.empty())
        {
            eventCallbacks.erase(eventCallbacksItem.first);
        }
    }
}

void EventBus::executeCallback(const CallbackItem& callbackItem, int eventCode, std::shared_ptr<void> data, int receiverId)
{
    try
    {
        callbackItem.callback(eventCode, data, receiverId);
    }
    catch (const std::bad_function_call& e)
    {
        // Handle exception or logging
    }
}
