
#include "runApp.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTcpServer>
#include <QTcpSocket>
#include <fstream>
#include <string>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <QScreen>
#include <QMessageBox>

struct JSCalleePrivate
{
    JSCalleePrivate()
    {
    }
};

runApp::runApp(QWidget * parent)
    : QWidget(parent)
    , ui(new Ui::runApp)
    , _Callee(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromLocal8Bit("webView测试"));

    // 在页面加载前注册 Javascript 访问对象
    auto callee = new JSCallee(this);

    ui->webView->RegisterObject("webViewTestObj", callee);

    connect(ui->webView, SIGNAL(loadProgress(int)), SLOT(onLoadProgress(int)));
    connect(ui->webView, SIGNAL(titleChanged(QString)), SLOT(onTitleChanged(QString)));
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), SLOT(onUrlChanged(QUrl)));

    onLoadProgress(ui->webView->LoadProgress());

    _Callee = callee;

    // QString currentdir = QDir::currentPath();
    ui->webView->setUrl(QUrl(callee->HomePage()));
    // ui->webView->setUrl(QUrl::fromUserInput(currentdir+"/"+callee->HomePage()));

    bool isok = ui->webView->IsRegisterObject("webViewTestObjObj");
}

runApp::~runApp()
{
    delete ui;
}

void runApp::onTitleChanged(const QString & title)
{
    setWindowTitle(title);
}

void runApp::onUrlChanged(const QUrl & url)
{
    QString text = url.toString();
    ui->lineEdit_Url->setText(text);
    ui->lineEdit_Url->setCursorPosition(0);
    if (text.isEmpty())
    {
        ui->lineEdit_Url->setFocus();
    }
}

void runApp::on_lineEdit_Url_returnPressed()
{
    QString text = ui->lineEdit_Url->text();
    // 当以"js:"开头时, 调用页面脚本
    if (text.startsWith("js:", Qt::CaseInsensitive))
    {
        ui->webView->Browser()->runJavaScript(text.mid(3), [this](const QVariant & v) {
            // 返回最后一条语句的值
            QString r = v.toString();
            if (!r.isEmpty())
            {
                QMessageBox::information(this, tr("Javascript"), r, tr("ok"));
            }
        });
    }
    else
    {
        QUrl url(QUrl::fromUserInput(text));
        if (url.isValid())
        {
            ui->webView->setUrl(url);
            ui->webView->setFocus();
        }
        onUrlChanged(ui->webView->url());
    }
}

void runApp::on_pushButton_clicked()
{
    QString    path = "data/test.png";
    QByteArray arr  = path.toUtf8();
    QPixmap    pixmap(arr);
    QByteArray imageData;
    QBuffer    buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    QString imageDataBase64 = QString(imageData.toBase64());
    ui->webView->Browser()->runJavaScript(QString("setImage('%1')").arg(imageDataBase64));
}

void runApp::onLoadProgress(int progress)
{
    // 显示加载进度
    progress = progress < 100 ? progress : 0;
    ui->progressBar->setValue(progress);
    ui->progressBar->setVisible(progress > 0);
}

JSCallee::JSCallee(QObject * parent)
    : QObject(parent)
    , _p(new JSCalleePrivate)
{
    QString currentDir = QDir::currentPath();
    QString path       = currentDir + "/html/test.html";
    _HomePage          = path;
    //_HomePage          = "http://huaru.com.cn/";
}

JSCallee::~JSCallee()
{
    if (_p)
    {
        delete _p;
        _p = nullptr;
    }
}

void JSCallee::QuitApp()
{
    QApplication::quit();
}

QString JSCallee::HomePage() const
{
    return _HomePage;
}

void JSCallee::SetHomePage(const QString & url)
{
    // 任意一端修改属性均会执行此方法, 同步时不会重复执行
    if (_HomePage != url)
    {
        _HomePage = url;
        _Settings.setValue(tr("HomePage"), url);
        emit HomePageChanged(url);
    }
}

bool JSCallee::CanLogin(QString user, QString pswd)
{
    if (user != "" && pswd != "")
    {
        return true;
    }
    return false;
}
