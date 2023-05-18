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
		//���ڴ洢�ص������������Ϣ�������ص����������ȼ�������ָ��ȡ�
        std::function<void(int, void*, int)> callback;  // �ص�����
        int                                  priority;  // ���ȼ�
        void*                                rawObject; // ����ָ�룬���ڸ��ݶ���ȡ�������¼�
    };

    struct EventCallbacksItem
    {
		//���ڴ洢�¼��Ͷ�Ӧ�Ļص������б�
        std::vector<CallbackItem> callbacks; // �ص������б�
    };

    struct EventQueueItem
    {
		//��ʾ��������¼�������¼��Ĵ��롢�������������б�ȡ�
        int                       eventCode;  // �¼�����
        void*                     data;       // ����
        int                       receiverId; // ������ID
        std::vector<CallbackItem> callbacks;  // �ص������б�
    };

    struct StickyEvent
    {
		//���ڴ洢ճ���¼�����Ϣ��ճ���¼���һ��������¼�������һֱ�������¼������У�ֱ��������
        int   eventCode;  // �¼�����
        void* data;       // ����
        int   receiverId; // ������ID
    };

    // ����һ���¼�,���¼��洢���¼�������
    void post(int eventCode, void* data, int receiverId);

    // ����һ���¼�
    void subscribe(int eventCode, std::function<void(int, void*, int)> callback, int priority = 0, void* rawObject = nullptr);

    // ȡ������һ���¼�
    void unsubscribe(int eventCode, std::function<void(int, void*, int)> callback);

    // ���ݶ���ȡ�������¼�
    void unsubscribeByRawObject(void* rawObject);

    // �����¼������е��¼�
    void handlePost();

    // ����һ��ճ���¼�
    void postStickyEvent(int eventCode, void* data, int receiverId);

    // ����ճ���¼�
    void handleStickyEvents();

private:
    std::unordered_map<int, EventCallbacksItem> eventCallbacks; // �洢�¼��Ͷ�Ӧ�Ļص������б�
    std::vector<EventQueueItem>                 eventQueue;     // �洢��������¼�����
    std::unordered_map<int, StickyEvent>        stickyEvents;   // �洢ճ���¼�����Ϣ

    // �����¼������е��¼���
    void handleEvents(const EventQueueItem& eventQueueItem);
};
