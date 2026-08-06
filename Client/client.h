#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "protocol.h"
#include "reshandler.h"

class Client : public QObject
{
    Q_OBJECT

public:
    // 单例模式，获取对象的静态函数
    static Client& getInstance();
    ~Client();

//    void sendTestData(QString strTestMessage);
    void reqLogin(QString strName, QString strPwd);
    void reqRegister(QString strName, QString strPwd);
    void reqFindUser(QString strName);
    void reqAddFriend(QString strTarName);
    void reqAddFriendAgree(const char* caCurName, const char* caTarName);
    void reqDeleteFriend(QString strTarName);
     void reqChat(QString strTarName,QString strmsg);
    ResHandler *getResHandler();
    QString m_strLoginName;
    QString m_strRootPath;

    void reqFlushFriend();
    void reqMKDir(QString strPath,QString strDirName);
    void reqUploadFileInit(QString strPath, QString strF);
    void reqFlushFile(QString strPath);
signals:
     void signalLoginResult(bool res);
     void signalRegisterResult(bool res);
     void signalAddFriendResult(bool res);
     void signalFindUserResult(const char* caName, bool res);
     void signalAddFriendResendResult(const char* caName, const char* caName2);
     void signalAddFriendAgreeResult(const char* caName, const char* caName2);
private slots:
    void onConnect2Server();
    void onRecvMsg();

private:
    QTcpSocket m_socket;
    QString m_strIp = "";
    quint16 m_ushortPort = 0;
    QByteArray m_baSocketBuffer;
    ResHandler* rh;
    // 单例模式，将构造函数移入private，删除拷贝构造函数和赋值
    explicit Client(QObject *parent = nullptr);
    Client (const Client& instance) = delete;
    Client& operator = (const Client&) = delete;
    void loadConfig();
    void connect2Server();
    void handleMsg(PDU *pdu);

};

#endif // CLIENT_H
