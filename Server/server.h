#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "tcpserver.h"

class Server : public QObject
{
    Q_OBJECT
public:
    // 单例模式，获取对象的静态函数
    static Server& getInstance();
    void reSocketFromTcpServer(TcpSocket *socket);
    TcpServer m_server;
    QString m_strRootPath;
signals:

private:

    QString m_strIp = "";
    quint16 m_ushortPort = 0;
    // 单例模式，将构造函数移入private，删除拷贝构造函数和赋值
    explicit Server(QObject *parent = nullptr);
    Server (const Server& instance) = delete;
    Server& operator = (const Server&) = delete;
    void loadConfig();

};

#endif // SERVER_H
