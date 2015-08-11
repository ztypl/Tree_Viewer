#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    //w.connectDB();
    //w.queryDB();
    //w.ButtomDock(QVector3D(1.111,1.122,1.313),QVector3D(2.111,2.212,2.313),QVector2D(3.211,3.212),"persimmon");

    w.show();

    return a.exec();
}
