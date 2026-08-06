#include "friendwindow.h"
#include "loginwindow.h"
#include <client.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    //自动缩放
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
//    Client client;
    //创建单例
    Client::getInstance();
    Loginwindow w;
    w.show();
    return a.exec();
}
