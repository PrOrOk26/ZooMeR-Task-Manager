#include "mainwin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget mainWindow;

    QApplication::setQuitOnLastWindowClosed(false);

    mainWindow.show();

    return a.exec();
}
