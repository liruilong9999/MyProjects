
#ifndef LWEBVIEW_H__
#define LWEBVIEW_H__

#include <QObject>
#include <QWebChannel>
#include <QWebEnginePage>

class LWebView : public QObject
{
    Q_OBJECT
public:
    explicit LWebView(QObject* parent = nullptr);
    ~LWebView();

signals:
    void recvdMsg(const QString& msg);

public:
    void sendMsg(QWebEnginePage* page, const QString& msg);
public slots:
    void onMsg(const QString& msg);
};

#endif
