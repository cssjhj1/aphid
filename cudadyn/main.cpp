#include <QApplication>

#include "window.h"

//! [0]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.resize(800, 600);
	window.show();
    return app.exec();
}
//! [0]
