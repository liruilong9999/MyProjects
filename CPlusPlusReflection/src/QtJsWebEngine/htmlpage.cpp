#include "htmlpage.h"
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
HtmlPage::HtmlPage(QWidget *parent) :
   QWebEngineView(parent)
{
    QWebEngineScript script;
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setSourceCode(Loadtext(":/qwebchannel.js"));
    //if (_Isok = !script.isNull())
    {
        QWebEngineProfile*          file       = QWebEngineProfile::defaultProfile();
        QWebEngineScriptCollection* collection = file->scripts();
        collection->insert(script);
    }
    // Cached X/Y coordinates
    sX = 0 ; sY = 0 ;
    // Set up the communications channel
    this->page()->setWebChannel(&channel) ;
    channel.registerObject("widget", WebObjectInstance::instance());
    connect(WebObjectInstance::instance(),SIGNAL(signalCursorMoved(int,int)),this,SLOT(slotJsUpdated(int,int)));
    // Set the page content
    setUrl(QUrl("qrc:/index.html")) ;
}

HtmlPage::~HtmlPage()
{
}

void HtmlPage::slotJsUpdated(int x, int y)
{
    qDebug("slotJsUpdated");
     addDot(x,y,6,"#FFFFFF");
}

// Call to add a dot at the given x/y coordinate
void HtmlPage::addDot(int x, int y, int sz, QString colour)
{
    QString command = QString("addDot(%1,%2,%3,'%4');").arg(x).arg(y).arg(sz).arg(colour) ;
    page()->runJavaScript(command) ;
}

