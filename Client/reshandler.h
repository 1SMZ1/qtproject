#ifndef RESHANDLER_H
#define RESHANDLER_H

#include <QObject>
#include "protocol.h"

class ResHandler : public QObject
{
    Q_OBJECT
public:
    PDU* pdu;
    explicit ResHandler(QObject *parent = nullptr);
    void handleRegister();

    void handleLogin();
    void handleFindUser();
    void handleAddFriend();
    void handleAddFriendAgree();
    void handleAddFriendResend();
    void handleFlushFriend();
    void handleDeleteFriend();
    void handleChat();
    void handleMKDir();
    void handleUploadFileData();
signals:
    void signalLoginResult(bool res);
    void signalRegisterResult(bool res);
    void signalAddFriendResult(bool res);
    void signalAddFriendAgreeResult(bool res);
    void signalFindUserResult(const char* caName, int res);
    void signalAddFriendResendResult(const char* caName, const char* caName2);
    void signalFlushFriendResult(QStringList strFriendList);
    void signalDeleteFriendResult(bool res);
    void signalChatResult(QString strName,QString caMsg);
    void signalMKDirResult(bool res);
    void signalUploadDataReault(bool res);
};

#endif // RESHANDLER_H
