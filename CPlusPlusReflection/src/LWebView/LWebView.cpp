/*
�����������ڻ���
1.����C++/Qtʵ��
2.����QWebView��QWebEngine��QWidget��QWebChannel��ɹ���
3.�ܹ���ת��ҳ
4.�ܹ�ͨ��C++����JavaScript�ӿڣ��ܹ�����JavaScript����C++�ӿڣ���������ܹ�ʹǰ��ҳ��ͱ���ֱ��ͨ��
5.�Ҽ����Ե���(��ȸ������F12����̨)����ջ��棬ˢ��
6.��Ҫдһ�����׵�html�ļ���Ϊ����
*/

#include "LWebView.h"

 LWebView::LWebView(QObject* parent /*= nullptr*/)
{
}

LWebView::~LWebView()
{
}

void LWebView::sendMsg(QWebEnginePage* page, const QString& msg)
{
    page->runJavaScript(QString("recvMessage('%1');").arg(msg));
}

void LWebView::onMsg(const QString& msg)
{
    emit recvdMsg(msg);
}
