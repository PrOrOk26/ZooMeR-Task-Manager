#include "mainwin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget mainWindow;



    mainWindow.show();

    return a.exec();
}
