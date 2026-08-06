#include "client.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QString>
#include "protocol.h"
#include "reshandler.h"

// 单例模式，获取对象的静态函数
Client& Client::getInstance()
{
    static Client instance;
    return instance;
}

ResHandler* Client::getResHandler()
{
    return rh;
}

Client::~Client()
{
    delete rh;
}

Client::Client(QObject *parent) : QObject(parent)
{
    rh = new ResHandler();
    loadConfig();
    connect2Server();
}

void Client::loadConfig()
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

void Client::connect2Server()
{
    // 请求服务器链接
    // 127.0.0.1 本机IP地址
    // 必须与服务器ip地址和端口号一致
    // m_socket.connectToHost(QHostAddress("127.0.0.1"), 2590);
    m_socket.connectToHost(QHostAddress(m_strIp), m_ushortPort);

    // 怎么知道是否成功连接
    // 1、把 connected 信号，接到槽函数上
    // 2、在槽函数里面打印
    // connect(信号发送实体的指针，信号函数的指针，信号接收实体的指针，槽函数的指针)
    connect(&m_socket, &QTcpSocket::connected, this, &Client::onConnect2Server);

    // 为了能从socket读取服务端来的消息，也需要连接readyRead信号
    connect(&m_socket, &QTcpSocket::readyRead, this, &Client::onRecvMsg);
    qDebug() << "初始化";
}

void Client::onConnect2Server()
{
    qDebug() << "已连接到服务器";
//    sendTestData("helllo world!");
}

//void Client::sendTestData(QString strTestMessage)
//{
//    //创建PDU
//     PDU* pdu = mkPDU(strTestMessage.length());
//     //数据打包,放进柔性数组
//     memcpy(
//             pdu->caMsg,
//             strTestMessage.toStdString().c_str(),
//             strTestMessage.toStdString().length()
//           );
//     //写进socket
//      m_socket.write((char*)pdu,pdu->uintTotalLen);
//      //释放内存,防止内存泄漏
//      free(pdu);
//      pdu=NULL;
//      m_socket.write(strTestMessage.toStdString().c_str(),strTestMessage.length());
//}
void Client::reqLogin(QString strName, QString strPwd)
{
    //创建PDU,这次使用固定长度部分,caData,所以柔性数组长度为0
     PDU* pdu = mkPDU();
    //数据打包,放入固定长度数组
     memcpy(pdu->caData,strName.toStdString().c_str(),strName.toStdString().length());
      memcpy(pdu->caData+32,strPwd.toStdString().c_str(),strPwd.toStdString().length());
      pdu->uintType=REQ_LOGIN;
      //写进socket
      m_socket.write((char*)pdu,pdu->uintTotalLen);
      m_socket.flush();
      //释放内存,防止内存泄漏
      free(pdu);
      pdu=NULL;
}
void Client::reqRegister(QString strName, QString strPwd)
{
    //创建PDU,这次使用固定长度部分,caData,所以柔性数组长度为0
     PDU* pdu = mkPDU();
     //数据打包,放入固定长度数组
      memcpy(pdu->caData,strName.toStdString().c_str(),strName.toStdString().length());
      memcpy(pdu->caData+32,strPwd.toStdString().c_str(),strPwd.toStdString().length());
      pdu->uintType=REQ_REGISTER;
       //写进socket
       m_socket.write((char*)pdu,pdu->uintTotalLen);
       //释放内存,防止内存泄漏
       free(pdu);
       pdu=NULL;
}
void Client::onRecvMsg()
{
    //接收服务器的数据
    qDebug()<<"onRecvMsg() 接收到信息的长度"<<m_socket.bytesAvailable();

    QByteArray data = m_socket.readAll();
    //m_baSocketBuffer是一个成员变量
    //从缓存里可能剩余没解析完的半个pdu,通过append的形式拼接起来形成完整的pdu
    m_baSocketBuffer.append(data);

    while(true)
    {
        //先判断长度至少要大于一个无柔性数组的pdu
        //如果不满足条件,则不处理,等下一波拼成一个完整的再处理
        //这样做的目的是为了确保缓存强转成pdu后,柔性数组以外的所有字段正确可用
        if((unsigned int)m_baSocketBuffer.size()<sizeof(PDU))
        {
            break;
        }
        // 把缓存强行转成pdu，拿到一个pdu的总长度
        // 如果剩余字节数这个pdu应有的长度，则不处理，等下一波拼成一个完整的在处理
        PDU* pdu = (PDU*) m_baSocketBuffer.data();
        if ((unsigned int) m_baSocketBuffer.size() < pdu->uintTotalLen)
        {
            break;
        }
        //处理一个完整的pdu
        //粘包问题无需考虑,因为多余部分访问不到
        handleMsg(pdu);

        //处理完的消息从buffer移除
        m_baSocketBuffer.remove(0,pdu->uintTotalLen);
    }

}

void Client::handleMsg(PDU *pdu)
{
    printPDU(pdu);
    rh->pdu = pdu;
    switch (pdu->uintType)
    {
    // 注册的返回
    case RECV_REGISTER:
    {
        rh->handleRegister();
        break;
    }
    // 登录的返回
    case RECV_LOGIN:
    {
        rh->handleLogin();
        break;
    }
    // 查询用户的返回
    case RECV_FIND_USERNAME:
    {
        rh->handleFindUser();
        break;
    }
    //添加好友
    case RECV_ADD_FRIEND:
    {
        rh->handleAddFriend();
        break;
    }
    // 添加好友转发
    case REQ_ADD_FRIEND:
    {
        rh->handleAddFriendResend();
        break;
    }
    //添加好友同意
    case RECV_ADD_FRIEND_AGREE:
    {
        rh->handleAddFriendAgree();
        break;
    }
    case RECV_FLUSH_FRIEND:
    {
        rh->handleFlushFriend ();
        break;
    }
    case RECV_DELETE_FRIEND:
    {
        rh->handleDeleteFriend ();
        break;
    }
    case REQ_CHAT:
    {
        rh->handleChat();
        break;
    }
    default:
    {
        qDebug() << "不支持的消息类型";
    }
    }
}
void Client::reqFindUser(QString strName)
{
    //创建PDU,这次使用固定长度部分,caData,所以柔性数组长度为0
     PDU* pdu = mkPDU();
     //数据打包,放入固定长度数组
      memcpy(pdu->caData,strName.toStdString().c_str(),strName.toStdString().length());
      pdu->uintType=REQ_FIND_USERNAME;
     //写进socket
      m_socket.write((char*)pdu,pdu->uintTotalLen);
     //释放内存,防止内存泄漏
     free(pdu);
     pdu=NULL;
}
void Client::reqAddFriend(QString strTarName)
{
    // 创建PDU，这次使用固定长度部分 caData，所以柔型数组长度为0
    PDU* pdu = mkPDU();
    // 数据打包，放进固定长度数组
    memcpy(pdu->caData, m_strLoginName.toStdString().c_str(), m_strLoginName.toStdString().length());
    memcpy(pdu->caData + 32, strTarName.toStdString().c_str(), strTarName.toStdString().length());
    pdu->uintType = REQ_ADD_FRIEND;
    // 写进socket
    m_socket.write((char*) pdu, pdu->uintTotalLen);
    //释放内存，防止内存泄漏
    free(pdu);
    pdu = NULL;
}

void Client::reqAddFriendAgree(const char *caCurName,const char *caTarName)
{
    PDU* pdu = mkPDU();
    memcpy(pdu->caData,caCurName,32);
    memcpy(pdu->caData+32,caTarName,32);
    pdu->uintType= REQ_ADD_FRIEND_AGREE;
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu=NULL;
}

void Client::reqFlushFriend()
{
    PDU* pdu = mkPDU();
    memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),m_strLoginName.toStdString().length());
    pdu->uintType=REQ_FLUSH_FRIEND;
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu=NULL;
}

void Client::reqMKDir(QString strPath, QString strDirName)
{
    PDU*pdu = mkPDU(strPath.toStdString().length());
    pdu->uintType = REQ_MKDIR;
    memcpy(pdu->caData,strDirName.toStdString().c_str(),32);
    memcpy(pdu->caData,strDirName.toStdString().c_str(),strPath.toStdString().length());
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu = NULL;
}

void Client::reqDeleteFriend(QString strTarName)
{
    PDU* pdu = mkPDU();
    pdu->uintType=REQ_DELETE_FRIEND;
    memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarName.toStdString().c_str(),32);
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu = NULL;
}

void Client::reqChat(QString strTarName, QString strmsg)
{
    PDU* pdu = mkPDU(strmsg.toStdString().size());
    pdu->uintType=REQ_CHAT;
    memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,strmsg.toStdString().c_str(),strmsg.toStdString().size());
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu = NULL;
}
void Client::reqFlushFile(QString strPath){
    PDU*pdu = mkPDU(strPath.toStdString().length());
    pdu->uintType = REQ_FLUSH_FILE;
    memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.length());
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu = NULL;

}
void Client::reqUploadFileInit(QString strPath,QString strFileName){
    PDU*pdu = mkPDU(strPath.toStdString().length());
    pdu->uintType = REQ_UPLOAD_FILE_INIT;
    memcpy(pdu->caData,strFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&iFileSize,sizeof (qint64));
    memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.length());
    printPDU(pdu);
    m_socket.write((char*)pdu,pdu->uintTotalLen);
    free(pdu);
    pdu = NULL;
}
