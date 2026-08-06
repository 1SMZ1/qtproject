#include "reshandler.h"
#include "client.h"
#include <QDebug>

ResHandler::ResHandler(QObject *parent) : QObject(parent)
{

}

void ResHandler::handleRegister()
{
    bool res = false;
    memcpy(&res, pdu->caData, sizeof (bool));
    qDebug() << "注册结果：" << res;
    // 使用信号和槽机制实现在loginwindow触发弹窗
    emit signalRegisterResult(res);
}

void ResHandler::handleLogin()
{
    bool res = false;
    memcpy(&res, pdu->caData, sizeof (bool));
    qDebug() << "登录结果：" << res;
    // 使用信号和槽机制实现在loginwindow触发弹窗
    emit signalLoginResult(res);
}

void ResHandler::handleFindUser()
{
    int res = false;
    // 取出用户名
    char caName[33];
    memset(caName, 0, sizeof (caName));
    memcpy(caName, pdu->caData, 32);
    memcpy(&res, pdu->caData + 32, sizeof (int));
    qDebug() << "查找用户" << caName;
    qDebug() << "查询结果：" << res;
    // 使用信号和槽机制实现在friendwindow触发弹窗
    emit signalFindUserResult(caName, res);
}

void ResHandler::handleAddFriend()
{
    int res;
    memcpy(&res,pdu->caData,sizeof (int));
    qDebug()<<"添加好友"<<res;
    emit signalAddFriendResult(res);
}
void ResHandler::handleAddFriendResend()
{
    // 取出用户名和目标用户名
    char caCurName[33];
    memset(caCurName, 0, sizeof (caCurName));
    memcpy(caCurName, pdu->caData, 32);
    char caTarName[33];
    memset(caTarName, 0, sizeof (caTarName));
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "添加好友转发" << caCurName << caTarName;
    emit signalAddFriendResendResult(caCurName, caTarName);
}
void ResHandler::handleAddFriendAgree()
{
    bool res = false;
    memcpy(&res,pdu->caData,sizeof (bool));
    qDebug()<<"添加好友结果:"<<res;
    emit signalAddFriendAgreeResult(res);
}
void ResHandler::handleFlushFriend ()
{
    int iSize = pdu->uintMsgLen/32;
    char caTmp[32] = {'\0'};
    QStringList friendList;
    for (int i=0; i<iSize; i++){
        memcpy(caTmp, pdu->caMsg+i*32, 32);
        friendList.append (caTmp);
    }
    //信号和槽函数记得在Friendwindow 的构造函数中connect
    emit signalFlushFriendResult (friendList);
}
void ResHandler::handleDeleteFriend (){
    bool res = false;
    memcpy(&res,pdu->caData,sizeof (bool));
    qDebug()<<"添加好友结果:"<<res;
    emit signalDeleteFriendResult(res);
}

void ResHandler::handleChat()
{
    char caCurName[33];
    memset(caCurName, 0, sizeof (caCurName));
    memcpy(caCurName, pdu->caData, 32);
    qDebug()<<"聊天"<<caCurName<<pdu->caMsg;
    emit signalChatResult(caCurName,pdu->caMsg);
}
void ResHandler::handleMKDir (){
    bool res = false;
    memcpy(&res,pdu->caData,sizeof (bool));
    qDebug()<<"创建文件夹的结果:"<<res;
    emit signalMKDirResult(res);
}
void ResHandler::handleUploadFileData()
{
    bool res = false;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"上传文件内容的结果"<<res;
    emit signalUploadDataReault(res);
}
