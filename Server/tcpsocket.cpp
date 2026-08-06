#include "tcpsocket.h"
#include "protocol.h"
#include "operatedb.h"
#include "server.h"
#include <QDebug>
TcpSocket::TcpSocket()
{
      mh = new MsgHandler;
      connect(this,&TcpSocket::readyRead,this,&TcpSocket::onRecvMsg);
      connect(this,&TcpSocket::disconnected, this, &TcpSocket::onDisconnected);
}
TcpSocket::~TcpSocket()
{
    delete mh;
}
//tcp断开时触发此槽函数
void TcpSocket::onDisconnected()
{
    //从数据库把登陆状态 is_online = 0
    //需要在登陆时把成员变量 m_strUserName 把 caName缓存起来,这里才能拿到用户名
    OperateDB::getInstance().handleOffline(m_strUserName.toStdString().c_str());

    //把自己销毁,从TcpServer的m_listTcpSocket列表中把自己去掉
    //不能直接调用 Server::getInstance().m_socket因为它是私有,需要包一层
    Server::getInstance().reSocketFromTcpServer(this);
}
void TcpSocket::onRecvMsg()//考虑粘包和拆包
{
    // 接收服务端的数据
    qDebug() << "onRecvMsg() sockct里面消息的总长度" << this->bytesAvailable();


    QByteArray data = this->readAll();
    // m_baSocketBuffer是一个成员变量
    // 这里缓存里可能剩余没解析完的半个pdu，通过append的形式拼接起来形成完整pdu
    m_baSocketBuffer.append(data);

    while (true)
    {
        // 先判断长度至少要大于一个无柔型数组的pdu
        // 如果不满足条件，则不处理，等下一波拼成一个完整的在处理
        // 这样做的目的是为了确保缓存强转成pdu后，柔性数组以外的所有字段正确可用
        if ((unsigned int) m_baSocketBuffer.size() < sizeof (PDU))
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
        // 处理一个完整pdu
        // 粘包问题无须考虑，因为多余部分我们访问不到
        PDU* recvPdu = handleMsg(pdu);
        if (recvPdu != NULL)
        {
            // 写进socket
            write((char*) recvPdu, recvPdu->uintTotalLen);
            printPDU(recvPdu);
            //释放内存，防止内存泄漏
            free(recvPdu);
            recvPdu = NULL;
        }

        // 处理完的消息从buffer移除
        m_baSocketBuffer.remove(0, pdu->uintTotalLen);
    }
}

//在服务端数据库查询账号密码
//登录则判断用户名和密码是否相同
//注册则判断用户名是否重复,不重复就加入数据库
PDU* TcpSocket::handleMsg(PDU *pdu)
{
    printPDU(pdu);

    mh->pdu = pdu;
    PDU* recvPdu = NULL;
    switch (pdu->uintType)
    {
    // 注册
    case REQ_REGISTER:
    {
        recvPdu = mh->handleRegister();
        break;
    }
    // 登录
    case REQ_LOGIN:
    {
        recvPdu = mh->handleLogin(m_strUserName);
        break;
    }
    // 查询用户
    case REQ_FIND_USERNAME:
    {
        recvPdu = mh->handleFindUserName();
        break;
    }
    case REQ_ADD_FRIEND:
    {
        recvPdu = mh->handleAddFriend();
        break;
    }
    case REQ_ADD_FRIEND_AGREE:
    {
        recvPdu = mh->handleAddFriendAgree();
        break;
    }
    case REQ_FLUSH_FRIEND:
    {
        recvPdu = mh->handleFlushFriend ();
        break;
    }
    case REQ_DELETE_FRIEND:
    {
        recvPdu = mh->handleDeleteFriend();
        break;
    }
    case REQ_CHAT:
    {
        recvPdu = mh->handleChat();
        break;
    }
    default:
        qDebug() << "不受支持的类型";
        recvPdu = NULL;
    }
    return recvPdu;
}
