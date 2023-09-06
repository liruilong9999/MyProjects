#include "runApp.h"

#include <Core/LWebView/LWebView.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LWebView::Init();

    runApp w;
    w.show();

    return a.exec();
}
