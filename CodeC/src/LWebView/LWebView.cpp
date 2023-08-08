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

#include <QFile>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

QString Loadtext(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 处理文件打开错误
        return QString();
    }

    QTextStream in(&file);
    QString     content = in.readAll();

    file.close();

    return content;
}

class LWebViewInitializer
{
public:
    LWebViewInitializer()
    {
        QWebEngineScript script;
        script.setInjectionPoint(QWebEngineScript::DocumentCreation);
        script.setWorldId(QWebEngineScript::MainWorld);
        script.setSourceCode(Loadtext(":/qwebchannel.js"));
        if (_Isok = !script.isNull())
        {
            QWebEngineProfile*          file       = QWebEngineProfile::defaultProfile();
            QWebEngineScriptCollection* collection = file->scripts();
            collection->insert(script);
        }
    }

public:
    bool isok() const { return _Isok; }

private:
    bool _Isok = true;
};

LWebView::LWebView(QObject* parent /*= nullptr*/)
{
    // 初始化LWebView
    LWebViewInitializer initializer;
    if (!initializer.isok())
    {
        qDebug() << "Failed to initialize LWebView";
        return;
    }
}

LWebView::~LWebView()
{

}

void LWebView::sendMsg(QWebEnginePage* page, const QString& msg)
{
    page->runJavaScript(QString("displayString('%1');").arg(msg));
}

void LWebView::onMsg(const QString& msg)
{
    emit recvdMsg(msg);
}
