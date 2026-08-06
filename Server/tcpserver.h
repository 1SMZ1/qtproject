#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcpsocket.h"
#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer();
    void removeSocket(TcpSocket *socket);
    void resend(char *caTarName, PDU *pdu);
protected:
    // 事件，当有客户端连接时触发
    virtual void incomingConnection(qintptr socketDescriptor) override;
    QList<TcpSocket *> m_listTcpSocket;
};

#endif // TCPSERVER_H
