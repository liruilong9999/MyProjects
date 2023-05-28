
#include "LWebView.h"
#include <QWebEngineView>

#include <QWidget>
//#include <QMainWindow>
#include "ui_runApp.h"

namespace Ui { class runApp; };

class runApp:public QWidget
{
	Q_OBJECT
public:
	runApp(QWidget * parent = nullptr);
	~runApp();


private:
    QWebEngineView* m_webView;
    QWebChannel*    m_webChannel;
    LWebView*       m_LwebView;

	Ui::runApp * ui;
};













// context
