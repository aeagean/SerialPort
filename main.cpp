/**********************************************************
Author: Qt君
微信公众号: Qt君(首发)
QQ群: 732271126
LICENSE: MIT
**********************************************************/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("SerialPort by Qt君");
    w.show();

    return a.exec();
}
