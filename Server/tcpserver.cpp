#include "tcpserver.h"
#include "tcpsocket.h"


#include <QDebug>

TcpServer::TcpServer()
{

}

// 事件，当有客户端连接时触发
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "成功与客户端链接";
    TcpSocket* pTcpSocket = new TcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_listTcpSocket.append(pTcpSocket);
    qDebug() << m_listTcpSocket;
}

void TcpServer::removeSocket(TcpSocket *socket)
{
    qDebug()<<"移除连接"<<socket;
    m_listTcpSocket.removeAll(socket);
    socket->deleteLater();
    socket = NULL;
    qDebug()<<m_listTcpSocket;
}
void TcpServer::resend(char *caTarName,PDU *pdu)
{
    if(caTarName == NULL || pdu == NULL){
        return;
    }
    for(int i=0;i<m_listTcpSocket.size();i++){
        if(m_listTcpSocket[i]->m_strUserName == caTarName)
        {
          printPDU(pdu);
          m_listTcpSocket[i]->write((char*)pdu,pdu->uintTotalLen);
          break;
        }
    }
}
