#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <vector>

template <typename EventData>
class EventBus
{
public:
    struct CallbackItem
    {
        using CallbackFunc = std::function<void(int, std::shared_ptr<EventData>, int)>;
        CallbackFunc              callback;
        int                       priority;
        std::weak_ptr<void>       context;
        std::chrono::milliseconds timeout;

        bool operator==(const CallbackItem& other) const
        {
            return callback == other.callback && context.lock() == other.context.lock();
        }
    };

    struct EventCallbacksItem
    {
        std::vector<CallbackItem> callbacks;
        std::mutex                mutex;
    };

    struct EventQueueItem
    {
        int                        eventCode;
        std::shared_ptr<EventData> data;
        int                        receiverId;
        std::vector<CallbackItem>  callbacks;
    };

    EventBus();
    ~EventBus();

    void post(int eventCode, std::shared_ptr<EventData> data, int receiverId);
    void subscribe(int eventCode, std::function<void(int, std::shared_ptr<EventData>, int)> callback, int priority = 0, std::shared_ptr<void> context = nullptr);
    void unsubscribe(int eventCode, std::function<void(int, std::shared_ptr<EventData>, int)> callback);
    void unsubscribeByContext(std::shared_ptr<void> context);
    void stop();

    void setBusTimeout(std::chrono::milliseconds timeout);
    void setEventQueueTimeout(std::chrono::milliseconds timeout);

private:
    std::unordered_map<int, EventCallbacksItem> eventCallbacks;
    std::queue<EventQueueItem>                  eventQueue;
    std::mutex                                  eventQueueMutex;
    std::condition_variable                     eventQueueCV;
    std::atomic<bool>                           running;
    std::thread                                 eventThread;
    std::chrono::milliseconds                   busTimeout;
    std::chrono::milliseconds                   eventQueueTimeout;

    void handlePost();
    void handleEvents(const EventQueueItem& eventQueueItem);
    void removeExpiredCallbacks();

    void executeCallback(const CallbackItem& callbackItem, int eventCode, std::shared_ptr<EventData> data, int receiverId);
};

template <typename EventData>
bool operator==(const typename EventBus<EventData>::CallbackItem& item1, const typename EventBus<EventData>::CallbackItem& item2)
{
    return item1.callback == item2.callback && item1.context.lock() == item2.context.lock();
}

template <typename EventData>
EventBus<EventData>::EventBus()
    : running(true), busTimeout(std::chrono::milliseconds::zero()), eventQueueTimeout(std::chrono::milliseconds::zero())
{
    eventThread = std::thread(&EventBus::handlePost, this);
}

template <typename EventData>
EventBus<EventData>::~EventBus()
{
    stop();
}

template <typename EventData>
void EventBus<EventData>::post(int eventCode, std::shared_ptr<EventData> data, int receiverId)
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

template <typename EventData>
void EventBus<EventData>::subscribe(int eventCode, std::function<void(int, std::shared_ptr<EventData>, int)> callback, int priority, std::shared_ptr<void> context)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    CallbackItem                callbackItem = {callback, priority, context, std::chrono::milliseconds::zero()};
    eventCallbacks[eventCode].callbacks.push_back(callbackItem);
    std::sort(eventCallbacks[eventCode].callbacks.begin(), eventCallbacks[eventCode].callbacks.end(), [](const CallbackItem& a, const CallbackItem& b)
              { return a.priority > b.priority; });
}

template <typename EventData>
void EventBus<EventData>::unsubscribe(int eventCode, std::function<void(int, std::shared_ptr<EventData>, int)> callback)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventCode].mutex);
    auto&                       callbacks = eventCallbacks[eventCode].callbacks;
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    if (callbacks.empty())
    {
        eventCallbacks.erase(eventCode);
    }
}

template <typename EventData>
void EventBus<EventData>::unsubscribeByContext(std::shared_ptr<void> context)
{
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    for (auto& eventCallbacksItem : eventCallbacks)
    {
        std::lock_guard<std::mutex> lock(eventCallbacksItem.second.mutex);
        auto&                       callbacks = eventCallbacksItem.second.callbacks;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [&](const CallbackItem& item)
                                       { return item.context.lock() == context; }),
                        callbacks.end());
        if (callbacks.empty())
        {
            eventCallbacks.erase(eventCallbacksItem.first);
        }
    }
}

template <typename EventData>
void EventBus<EventData>::stop()
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

template <typename EventData>
void EventBus<EventData>::setBusTimeout(std::chrono::milliseconds timeout)
{
    busTimeout = timeout;
}

template <typename EventData>
void EventBus<EventData>::setEventQueueTimeout(std::chrono::milliseconds timeout)
{
    eventQueueTimeout = timeout;
}

template <typename EventData>
void EventBus<EventData>::handlePost()
{
    while (running)
    {
        EventQueueItem eventQueueItem;
        {
            std::unique_lock<std::mutex> lock(eventQueueMutex);
            if (eventQueueTimeout.count() > 0)
            {
                if (eventQueueCV.wait_for(lock, eventQueueTimeout, [&]
                                          { return !eventQueue.empty() || !running; }))
                {
                    if (!running)
                        return;
                    eventQueueItem = eventQueue.front();
                    eventQueue.pop();
                }
                else
                {
                    // Handle bus timeout event
                    // ...
                    continue;
                }
            }
            else
            {
                eventQueueCV.wait(lock, [&]
                                  { return !eventQueue.empty() || !running; });
                if (!running)
                    return;
                eventQueueItem = eventQueue.front();
                eventQueue.pop();
            }
        }
        handleEvents(eventQueueItem);
        removeExpiredCallbacks();
    }
}

template <typename EventData>
void EventBus<EventData>::handleEvents(const EventQueueItem& eventQueueItem)
{
    std::lock_guard<std::mutex> lock(eventCallbacks[eventQueueItem.eventCode].mutex);
    for (const CallbackItem& callbackItem : eventQueueItem.callbacks)
    {
        auto context = callbackItem.context.lock();
        if (context)
        {
            auto start   = std::chrono::steady_clock::now();
            auto end     = start + callbackItem.timeout;
            bool timeout = false;
            while (true)
            {
                if (busTimeout.count() > 0)
                {
                    auto now = std::chrono::steady_clock::now();
                    if (now > end)
                    {
                        timeout = true;
                        break;
                    }
                }
                executeCallback(callbackItem, eventQueueItem.eventCode, eventQueueItem.data, eventQueueItem.receiverId);
                if (callbackItem.timeout.count() == 0)
                {
                    break;
                }
            }
            if (timeout)
            {
                // Handle callback timeout event
                // ...
            }
        }
    }
}

template <typename EventData>
void EventBus<EventData>::removeExpiredCallbacks()
{
    for (auto& eventCallbacksItem : eventCallbacks)
    {
        std::lock_guard<std::mutex> lock(eventCallbacksItem.second.mutex);
        auto&                       callbacks = eventCallbacksItem.second.callbacks;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [&](const CallbackItem& item)
                                       { return item.context.expired(); }),
                        callbacks.end());
        if (callbacks.empty())
        {
            eventCallbacks.erase(eventCallbacksItem.first);
        }
    }
}

template <typename EventData>
void EventBus<EventData>::executeCallback(const CallbackItem& callbackItem, int eventCode, std::shared_ptr<EventData> data, int receiverId)
{
    try
    {
        callbackItem.callback(eventCode, data, receiverId);
    }
    catch (const std::exception& e)
    {
        // Handle callback exception
        // ...
    }
}
