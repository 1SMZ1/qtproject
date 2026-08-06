#include <QCoreApplication>
#include "server.h"
#include <operatedb.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //连接数据库
    OperateDB::getInstance().connect();
    // 创建单例
    Server::getInstance();
    return a.exec();
}
