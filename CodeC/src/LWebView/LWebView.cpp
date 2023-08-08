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

#include <QFile>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

QString Loadtext(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // �����ļ��򿪴���
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
    // ��ʼ��LWebView
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
