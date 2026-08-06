#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "protocol.h"
#include <QFile>
#include <QString>


class MsgHandler
{
public:
    PDU* pdu;
    MsgHandler();
    PDU *handleRegister();
    PDU *handleLogin(QString &name);
    PDU *handleFindUserName();
    PDU *handleAddFriend();
    PDU *handleAddFriendAgree();
    PDU *handleFlushFriend();
    PDU *handleDeleteFriend();
    PDU *handleChat();
    PDU *handleMKDir();
    PDU *handUploadData();
    QFile m_fUploadFile;
    qint64 m_iUploadReceived;
    qint64 m_iUploadSize;
};

#endif // MSGHANDLER_H
