#include "server.h"

#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include "tcpsocket.h"

// 单例模式，获取对象的静态函数
Server &Server::getInstance()
{
    static Server server;
    return server;
}

Server::Server(QObject *parent) : QObject(parent)
{
    loadConfig();
    m_server.listen(QHostAddress(m_strIp), m_ushortPort);
}

//包一层,以便外界使用
void Server::reSocketFromTcpServer(TcpSocket* socket)
{
    m_server.removeSocket(socket);
}

void Server::loadConfig()
{
    QFile file(":/config.ini");
    if (file.open(QIODevice::ReadOnly))
    {
        QString strRawConfig = QString(file.readAll());
        // qDebug() << strRawConfig;
        QStringList strList = strRawConfig.split("\r\n");
        m_strIp = strList[0];
        m_ushortPort = strList[1].toUShort();
         m_strRootPath=strList[2];
        qDebug() << "ip =" << m_strIp << "port =" << m_ushortPort<<"root path ="<<m_strRootPath;
    }
    else
    {
        qDebug() << "打开文件失败！";
    }
}
