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
    //C++调用js //上传图片
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
    // 增加属性改变信号可自动将 C++ 中的改变同步到 Javascript
    void HomePageChanged(const QString & url);

public slots:
	//js调用C++
	// 1.无返回值的调用
	void QuitApp();
	// 2.有返回值的调用
	// 登录按钮调用
	bool CanLogin(QString user, QString pswd);

public:
    //获取url
    QString HomePage() const;
    //设置url
    void SetHomePage(const QString & url);

private:
    QSettings _Settings;
    QString   _HomePage;

    JSCalleePrivate * _p;
};

#endif
