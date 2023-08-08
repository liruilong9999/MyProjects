#include "runApp.h"

#include <LWebView/LWebView.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LWebView::Init();

    runApp w;
    w.show();

    return a.exec();
}
