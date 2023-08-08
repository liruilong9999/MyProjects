
#include <QApplication>

#include "BrowserWindow.h"
#include "runApp.h"

//#pragma execution_character_set("utf-8")

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    // qRegisterMetaType<BrowserWindow*>("BrowserWindow*");
    runApp a;
    a.show();
    return app.exec();
}
