
#include "runApp.h"

runApp::runApp(QWidget* parent)
    : m_webView(nullptr)
    , m_webChannel(nullptr)
    , m_LwebView(nullptr)
{
    ui = new Ui::runApp();
    ui->setupUi(this);
    m_webView    = new QWebEngineView(this);
    m_webChannel = new QWebChannel(this);
    m_LwebView   = new LWebView(this);

    m_webChannel->registerObject("context", m_LwebView);
    ui->webEngineView->page()->setWebChannel(m_webChannel);

    bool isconnected = connect(m_LwebView, &LWebView::recvdMsg, this, [this](const QString& msg)
            { ui->textBrowser->setText(QString("reseived msg :%1").arg(msg)); }

    );

    ui->webEngineView->load(QUrl::fromLocalFile("D:\\Code\\GitCode\\MyProjects\\CPlusPlusReflection\\bin\\html\\test.html"));
}

runApp::~runApp()
{
}

void runApp::on_pushButton_clicked()
{
	m_LwebView->sendMsg(m_webView->page(),"dfa3412341234");
}
