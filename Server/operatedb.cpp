#include "operatedb.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <protocol.h>
OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}
OperateDB::~OperateDB()
{
    //断开数据库连接
    QSqlDatabase db = QSqlDatabase :: database();
    db.close();
}
OperateDB::OperateDB(QObject *parent) : QObject(parent)
{

}
//连接到数据库
void OperateDB::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    // 下面这几项：根据同学们实际情况来填写
    db.setHostName("localhost"); // 127.0.0.1
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("mysqlmysql");
    db.setDatabaseName("2507_userinfo");

    //调用open检查是否成功
    if(db.open())
    {
        qDebug()<<"连接数据库成功";
    }
    else
    {
        qDebug()<<"连接数据库失败\n"<<db.lastError().text();
    }
}
bool OperateDB::handleRegister(char* caName, char* caPwd)
{
    if (caName == NULL || caPwd == NULL)
    {
        return false;
    }

    // 处理注册
    // 查询用户名是否已经被使用，不允许注册返回失败
    QString strFindSql = QString(
        "select * from users where name = '%1'"
        ).arg(caName);
    qDebug() << "查询用户名是否存在sql:" << strFindSql;

    // 执行sql语句
    QSqlQuery queryFind;
    if (!queryFind.exec(strFindSql))
    {
        qDebug() << "执行sql语句执行失败";
        return false;
    }

    // next函数，每次调用返回一个结果集中的结果
    // 如果结果集中有结果，则说明存在用户名，不能注册
    if (queryFind.next())
    {
        return false;
    }

    // 添加用户
    QString strAddSql = QString(
        "insert into users(name, password) values('%1', '%2')"
        ).arg(caName).arg(caPwd);
    qDebug() << "添加用户sql:" << strAddSql;

    QSqlQuery queryAdd;
    if (!queryAdd.exec(strAddSql))
    {
        qDebug() << "执行sql语句执行失败";
        return false;
    }
    qDebug() << "添加用户成功";
    return true;
}


bool OperateDB::handleLogin(char* caName, char* caPwd)
{
    if (caName == NULL || caPwd == NULL)
    {
        return false;
    }

    // 处理登录

    // 从数据库查询：用户名和密码都符合，登录成功，否则登录失败
    // 构造sql语句
    QString strFindUser = QString("select * from users where name='%1' and password='%2'").arg(caName).arg(caPwd);

    // 执行sql语句
    QSqlQuery queryFind;
    if (!queryFind.exec(strFindUser))
    {
        qDebug() << "执行sql语句执行失败" << strFindUser;
        return false;
    }

    // 查看结果集是否有结果
    if (!queryFind.next())
    {
        return false;
    }

    // 构造sql语句
    QString strSetOnline = QString("update users set is_online=1 where name='%1'").arg(caName);
    QSqlQuery querySetOnline;
    if (!querySetOnline.exec(strSetOnline))
    {
        qDebug() << "执行sql语句执行失败" << strSetOnline;
    }

    // 返回是否登录成功
    return true;
}

void OperateDB::handleOffline(const char* caName)
{
    // 构造sql语句
    QString strSetOnline = QString(
                "update users set is_online=0 where name='%1'").arg(caName);
    QSqlQuery querySetOnline;
    if (!querySetOnline.exec(strSetOnline))
    {
        qDebug() << "执行sql语句执行失败" << strSetOnline;
    }
}

int OperateDB::handleFindUser(const char* caName)
{
    if (caName == NULL)
    {
        return FIND_USER_FAIL_NAME_ERR;
    }
    QString strFindUser = QString(
         "select is_online from users where name = '%1'"
    ).arg(caName);
    QSqlQuery queryFindUser;
    if (!queryFindUser.exec(strFindUser))
    {
        qDebug() << "执行sql语句执行失败" << strFindUser;
        return FIND_USER_FAIL_SQL_ERR;
    }
    if (!queryFindUser.next())
    {
        qDebug() << "未找到用户" << strFindUser;
        return FIND_USER_FAIL_NOT_FOUND;
    }
    return queryFindUser.value(0).toInt();
}
int OperateDB::handleAddFriend(const char *caCurName, const char *caTarName)
{
    if (caCurName == NULL || caTarName == NULL) {
        return ADD_FRIEND_FAIL_NAME_ERR;
    }
    QString strSql = QString(
                R"(select * from friend
                where
                (
                  user_id=(select id from users where name='%1')
                  and
                  friend_id=(select id from users where name='%2')
                ) or
                (
                  user_id=(select id from users where name='%2')
                  and
                  friend_id=(select id from users where name='%1')
                );)"
                ).arg(caCurName).arg(caTarName);
    QSqlQuery q;
    if (!q.exec(strSql))
    {
        qDebug() << "执行查询是否已经是好友的sql语句执行失败" << strSql;
        return ADD_FRIEND_FAIL_SQL_ERR;
    }
    if (q.next())
    {
        qDebug() << "双方已经是好友" << strSql;
        return ADD_FRIEND_FAIL_NOT_FOUND;
    }
    strSql = QString("select is_online from users where name='%1'").arg(caTarName);
    if (!q.exec(strSql))
    {
        qDebug() << "执行查询对方是否在线的sql语句执行失败" << strSql;
        return ADD_FRIEND_FAIL_SQL_ERR;
    }
    if (!q.next())
    {
        qDebug() << "对方名字不存在" << strSql;
        return ADD_FRIEND_FAIL_NOT_FOUND;
    }
    return q.value(0).toInt();
}

bool OperateDB::handleAddFriendAgree(const char *caCurName, const char *caTarName)
{
    if(caCurName == NULL || caTarName == NULL){
        return false;
    }
    QString strSql =  QString(R"(
      insert into friend (user_id,friend_id)
      select u1.id,u2.id
      from users u1,users u2
      where u1.name= '%1' and u2.name='%2';
                              )").arg(caCurName).arg(caTarName);
    qDebug()<<"添加好友SQL语句"<<strSql;
    QSqlQuery q;
    return q.exec(strSql);
}
QStringList OperateDB::handleFlushFriend (const char *caName)
{
    QStringList result;
    result.clear ();
    if (caName == NULL) {
        return result;
    }
    QString strSql = QString (R"(
    select name from users where is_online=1 and id in (
    select friend_id from friend where user_id =(select id from users where name='%1')
    union
    select user_id from friend where friend_id =(select id from users where name='%1')
    );
)").arg (caName);
    QSqlQuery q;
    if (!q.exec(strSql))
{
    qDebug()<<"执行查询在线好友名的sql语句执行失败"<<strSql;
    return result;
}
    while(q.next ()) {
    result.append (q.value (0).toString ());
}
    return result;
}

bool OperateDB::handleDeleteFriend(const char *caCurName, const char *caTarName)
{
    if(caCurName == NULL || caTarName == NULL){
        return false;
    }
    QString strSql = QString(
                R"(
                select * from friend
                where
                (
                  user_id=(select id from users where name='%1')
                  and
                  friend_id=(select id from users where name='%2')
                ) or
                (
                  user_id=(select id from users where name='%2')
                  and
                  friend_id=(select id from users where name='%1')
                );)"
                ).arg(caCurName).arg(caTarName);
    QSqlQuery q;
    if (!q.exec(strSql))
    {
        qDebug() << "执行查询是否已经是好友的sql语句执行失败" << strSql;
        return false;
    }
    if (!q.next())
    {
        qDebug() << "好友不存在" << strSql;
        return false;
    }
    strSql = QString(
                    R"(delete from friend
                    where
                    (
                      user_id=(select id from users where name='%1')
                      and
                      friend_id=(select id from users where name='%2')
                    ) or
                    (
                      user_id=(select id from users where name='%2')
                      and
                      friend_id=(select id from users where name='%1')
                    );)"
                    ).arg(caCurName).arg(caTarName);

    if (!q.exec(strSql))
    {
        qDebug() << "执行删除的sql语句执行失败" << strSql;
        return false;
    }
    return true;
}
