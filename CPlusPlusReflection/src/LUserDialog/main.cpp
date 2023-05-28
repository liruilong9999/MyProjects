#include "LUserDialog.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

     CustomWindow window;
     window.setWindowTitle("Custom Window");
     //window.resize(400, 300);
     window.show();


    return app.exec();
}
