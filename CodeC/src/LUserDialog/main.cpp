#include "LUserDialog.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

     CustomWindow window;
     window.setWindowTitle("Custom Window");
     window.resize(1920, 1080);
     window.show();


    return app.exec();
}
