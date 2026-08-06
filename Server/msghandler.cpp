#include "msghandler.h"
#include "protocol.h"
#include "operatedb.h"
#include <cstring>
#include <QDebug>
#include <QDir>
#include "tcpserver.h"
#include "server.h"
// 1、建立 MsgHandler 类，有一个成员变量叫 pdu
// 2、将对应功能在 MsgHandler 建立成员函数，将原逻辑迁移
// 3、write逻辑，提到onRecvMsg里面

MsgHandler::MsgHandler()
{

}

PDU* MsgHandler::handleRegister()
{
    // 取出用户名密码
    char caName[33];
    memset(caName, 0, sizeof (caName));
    memcpy(caName, pdu->caData, 32);
    char caPwd[33];
    memset(caPwd, 0, sizeof (caPwd));
    memcpy(caPwd, pdu->caData + 32, 32);
    qDebug() << "注册用户" << caName << caPwd;

    // 查询数据库
    bool res = OperateDB::getInstance().handleRegister(
        caName, caPwd
    );

    qDebug() << "注册用户结果：" << res;
    if(res){
        QDir dir;
        QString strUserpath = QString("%1/%2").arg(Server::getInstance().m_strRootPath).arg(caName);
        qDebug()<<"创建用户同名文件夹"<<strUserpath;
        bool ret = dir.mkdir(strUserpath);
        qDebug()<<"mkdir ret"<<ret;
    }
    // 相客户端返回结果
    // 创建PDU，这次使用固定长度部分 caData，所以柔型数组长度为0
    PDU* recvPdu = mkPDU();
    // 数据打包，放进固定长度数组
    memcpy(recvPdu->caData, &res, sizeof (bool));
    recvPdu->uintType = RECV_MKDIR;
    return recvPdu;
}

PDU* MsgHandler::handleLogin(QString &m_strUserName)
{
    // 取出用户名密码
    char caName[33];
    memset(caName, 0, sizeof (caName));
    memcpy(caName, pdu->caData, 32);
    char caPwd[33];
    memset(caPwd, 0, sizeof (caPwd));
    memcpy(caPwd, pdu->caData + 32, 32);
    qDebug() << "用户登录" << caName << caPwd;

    // 查询数据库
    bool res = OperateDB::getInstance().handleLogin(
        caName, caPwd
    );
    // 打印返回结果
    qDebug() << "用户登录结果：" << res;

    // 缓存 caName 以便后续使用
    if (res)
    {
        m_strUserName = QString(caName);
    }

    // 相客户端返回结果
    // 创建PDU，这次使用固定长度部分 caData，所以柔型数组长度为0
    PDU* recvPdu = mkPDU();
    // 数据打包，放进固定长度数组
    memcpy(recvPdu->caData, &res, sizeof (bool));
    recvPdu->uintType = RECV_LOGIN;
    return recvPdu;
}

PDU* MsgHandler::handleFindUserName()
{
    // 取出用户名
    char caName[33];
    memset(caName, 0, sizeof (caName));
    memcpy(caName, pdu->caData, 32);
    qDebug() << "查找用户" << caName;

    // 查询数据库
    int res = OperateDB::getInstance().handleFindUser(caName);
    // 打印返回结果
    qDebug() << "查找用户结果：" << res;

    // 相客户端返回结果
    // 创建PDU，这次使用固定长度部分 caData，所以柔型数组长度为0
    PDU* recvPdu = mkPDU();
    // 数据打包，放进固定长度数组
    memcpy(recvPdu->caData, caName, 32);
    memcpy(recvPdu->caData + 32, &res, sizeof(int));
    recvPdu->uintType = RECV_FIND_USERNAME;
    return recvPdu;
}

PDU *MsgHandler::handleAddFriend()
{
    // 取出当前用户名和目标用户名
    char caCurName[33];
    memset(caCurName, 0, sizeof (caCurName));
    memcpy(caCurName, pdu->caData, 32);
    char caTarName[33];
    memset(caTarName, 0, sizeof (caTarName));
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "添加好友" << caCurName << caTarName;
    // 查询数据库
    int res = OperateDB::getInstance().handleAddFriend(caCurName, caTarName);
    // 打印返回结果
    qDebug() << "添加好友结果：" << res;
    if (res != 1) {
        PDU* recvPdu = mkPDU();
        // 数据打包，放进固定长度数组
        memcpy(recvPdu->caData, &res, sizeof (int));
        recvPdu->uintType = RECV_ADD_FRIEND;
        return recvPdu;
    } else {
        Server::getInstance().m_server.resend(caTarName, pdu);
        return NULL;
    }
}

PDU *MsgHandler::handleAddFriendAgree()
{
    // 取出当前用户名和目标用户名
    char caCurName[33];
    memset(caCurName, 0, sizeof (caCurName));
    memcpy(caCurName, pdu->caData, 32);
    char caTarName[33];
    memset(caTarName, 0, sizeof (caTarName));
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "添加好友" << caCurName << caTarName;
    //查询数据库
    bool res = OperateDB::getInstance().handleAddFriendAgree(caCurName, caTarName);
    // 打印返回结果
    qDebug() << "添加好友结果：" << res;
    PDU* recvPdu = mkPDU();
    // 数据打包，放进固定长度数组
    memcpy(recvPdu->caData, &res, sizeof (bool));
    recvPdu->uintType = RECV_ADD_FRIEND_AGREE;
    Server::getInstance().m_server.resend(caCurName, recvPdu);
    return recvPdu;
}

PDU *MsgHandler::handleFlushFriend ()
{
    //取出当前用户名和目标用户名
    char caCurName[33];
    memset (caCurName, 0, sizeof (caCurName));
    memcpy (caCurName, pdu->caData, 32);
    QStringList res =OperateDB::getInstance () .handleFlushFriend (caCurName);
    qDebug()<<"获取到的好友数量res.size()"<<res.size();
    PDU* recvPdu = mkPDU (res.size ()*32);
    recvPdu->uintType = RECV_FLUSH_FRIEND;
    for (int i=0; i<res.size (); i++){
        memcpy(recvPdu->caMsg+i*32,res[i].toStdString ().c_str (), 32);
        qDebug()<<"好友名res[i]"<<res[i];
    }
    return recvPdu;
}

PDU *MsgHandler::handleDeleteFriend()
{
    char caCurName[33];
    memset(caCurName, 0, sizeof (caCurName));
    memcpy(caCurName, pdu->caData, 32);
    char caTarName[33];
    memset(caTarName, 0, sizeof (caTarName));
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "删除好友" << caCurName << caTarName;
    //查询数据库
    bool res = OperateDB::getInstance().handleDeleteFriend(caCurName, caTarName);
    // 打印返回结果
    qDebug() << "删除好友结果：" << res;
    PDU* recvPdu = mkPDU();
    // 数据打包，放进固定长度数组
    memcpy(recvPdu->caData, &res, sizeof (bool));
    recvPdu->uintType = RECV_DELETE_FRIEND;
    return recvPdu;
}

PDU *MsgHandler::handleChat()
{
    char caTarName[33];
    memset(caTarName, 0, sizeof (caTarName));
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug()<<"聊天"<<caTarName;
    Server::getInstance().m_server.resend(caTarName,pdu);
    return NULL;
}
PDU *MsgHandler::handleMKDir(){
    char caDirName[33];
    memset(caDirName, 0, sizeof (caDirName));
    memcpy(caDirName, pdu->caData + 32, 32);
    QDir dir;
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(caDirName);
    qDebug()<<"handleMKDir strPath"<<strPath;
    bool res = dir.mkdir(strPath);
    qDebug()<<"mkdir res"<<res;
    PDU* recvPdu = mkPDU();
    memcpy(recvPdu->caData,&res,sizeof(bool));
    recvPdu->uintType = RECV_MKDIR;
    return recvPdu;
}
PDU*MsgHandler::handUploadData(){
    m_fUploadFile.write(pdu->caMsg,pdu->uintMsgLen);
    m_iUploadReceived += pdu->uintMsgLen;
    if(m_iUploadSize > m_iUploadReceived){
        return NULL;
    }
    m_fUploadFile.close();
    PDU* recvPdu = mkPDU();
    bool res = m_iUploadSize==m_iUploadReceived;
    memcpy(recvPdu->caData,&res,sizeof(bool));
    recvPdu->uintType = RECV_UPLOAD_FILE_DATA;
    return recvPdu;
}



















