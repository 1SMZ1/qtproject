#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    static OperateDB& getInstance();
    ~OperateDB();

    void connect();  
    bool handleRegister(char *caName, char *caPwd);
    bool handleLogin(char *caName, char *caPwd);
    void handleOffline(const char* caName);
    int handleFindUser(const char *caName);
    int handleAddFriend(const char *caCurName, const char *caTarName);
    bool handleAddFriendAgree(const char *caCurName, const char *caTarName);
    QStringList handleFlushFriend(const char *caName);
    bool handleDeleteFriend(const char *caCurName, const char *caTarName);
private:
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB (const OperateDB& instance) = delete;
    OperateDB& operator = (const OperateDB&) = delete;
signals:

};

#endif // OPERATEDB_H
