#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "protocol.h"
#include "msghandler.h"
#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
private:
    QByteArray m_baSocketBuffer;
    PDU* handleMsg(PDU *pdu);
    MsgHandler* mh;
public:
    TcpSocket();
    ~TcpSocket();
    QString m_strUserName;
protected slots:
    void onRecvMsg();
    void onDisconnected();
};

#endif // TCPSOCKET_H
