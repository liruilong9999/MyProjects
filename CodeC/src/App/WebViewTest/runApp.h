#ifndef __RUNAPP_H__20230629
#define __RUNAPP_H__20230629

#include <Core/LWebView/LWebView.h>
#include <QWebEngineView>
#include <QWidget>
#include <QObject>
#include <QSettings>
# include<QDialog>

#include "ui_runApp.h"

class JSCallee;
struct JSCalleePrivate;

namespace Ui {
class runApp;
};

class runApp : public QWidget
{
    Q_OBJECT
public:
    explicit runApp(QWidget * parent = nullptr);
    ~runApp();

public slots:
    //C++����js //�ϴ�ͼƬ
	void on_pushButton_clicked();

    void onTitleChanged(const QString & title);
    void onUrlChanged(const QUrl & url);
	void on_lineEdit_Url_returnPressed();
	void onLoadProgress(int progress);

private:
    Ui::runApp * ui;
    JSCallee *   _Callee;
};

class JSCallee : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString HomePage READ HomePage WRITE SetHomePage NOTIFY HomePageChanged)

public:
    explicit JSCallee(QObject * parent = nullptr);
    ~JSCallee();

signals:
    // �������Ըı��źſ��Զ��� C++ �еĸı�ͬ���� Javascript
    void HomePageChanged(const QString & url);

public slots:
	//js����C++
	// 1.�޷���ֵ�ĵ���
	void QuitApp();
	// 2.�з���ֵ�ĵ���
	// ��¼��ť����
	bool CanLogin(QString user, QString pswd);

public:
    //��ȡurl
    QString HomePage() const;
    //����url
    void SetHomePage(const QString & url);

private:
    QSettings _Settings;
    QString   _HomePage;

    JSCalleePrivate * _p;
};

#endif
