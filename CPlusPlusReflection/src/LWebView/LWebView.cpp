/*
完成浏览器窗口基类
1.利用C++/Qt实现
2.利用QWebView、QWebEngine、QWidget、QWebChannel完成功能
3.能够跳转网页
4.能够通过C++调用JavaScript接口，能够利用JavaScript调用C++接口，即浏览器能够使前端页面和本地直接通信
5.右键可以调试(如谷歌浏览器F12控制台)，清空缓存，刷新
6.需要写一个配套的html文件作为测试
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
