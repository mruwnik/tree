#include "glade.h"
#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(w.windowState() | Qt::WindowMaximized);
    w.show();

//    Glade glade;
//    glade.show();


    return a.exec();
}
