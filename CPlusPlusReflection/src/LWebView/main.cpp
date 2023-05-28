
#include <QApplication>

#include "runApp.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

	runApp a;
	a.show();

    return app.exec();
}
