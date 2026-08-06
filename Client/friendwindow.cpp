    #include "friendwindow.h"
#include "ui_friendwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
#include "client.h"
#include "protocol.h"
#include "reshandler.h"

FriendWindow::FriendWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendWindow)
{
    ui->setupUi(this);
    connect(Client::getInstance().getResHandler(), &ResHandler::signalFindUserResult, this, &FriendWindow::onFindUserResult);
    m_pChat = new Chat;
    connect(Client::getInstance().getResHandler(), &ResHandler::signalAddFriendResult, this, &FriendWindow::onAddFriendResult);
    connect(Client::getInstance().getResHandler(), &ResHandler::signalAddFriendResendResult, this, &FriendWindow::onAddFriendResendResult);
    connect(Client::getInstance().getResHandler(), &ResHandler::signalAddFriendAgreeResult, this, &FriendWindow::onAddFriendAgreeResult);
    connect(Client::getInstance().getResHandler(), &ResHandler::signalFlushFriendResult, this, &FriendWindow::updateListWidget);
    connect(Client::getInstance().getResHandler(), &ResHandler::signalDeleteFriendResult, this, &FriendWindow::onDeleteFriendResult);
}

FriendWindow::~FriendWindow()
{
    delete ui;
    delete m_pChat;
}

// 一个非常常见的客户端和服务端的交互流程（以查询用户举例）
// 1、点击界面控件，调用功能模块相关功能处理请求 FriendWindow::on_btnFindUser_clicked
// 2、功能模块构造请求消息，发送给服务端 Client::reqFindUser
// 3、服务端解析请求消息，交给对应模块处理逻辑 TcpSocket::handleMsg
// 4、对应模块处理逻辑后，返回处理结果 OperateDB::handleFindUser
// 5、服务端构造返回消息，回应客户端请求 TcpSocket::handleMsg
// 6、客户端解析返回消息，发送信号将参数传递给窗口 Client::handleMsg
// 7、窗口再根据信号传递的参数，显示表现 FriendWindow::onFindUserResult

// 同学们可以照着登录写过的代码，对应逻辑复制粘贴下来完成新的需求
// ！！千万不要忘记复制下来的代码，要检查每一行是否正确！！

void FriendWindow::on_btnFindUser_clicked()
{
    QString text = QInputDialog::getText(this,"搜索","用户名");
    qDebug() << text;
    if(text.isEmpty())
    {
        QMessageBox::warning(this,"提示","用户名为空");
        return;
    }
    Client::getInstance().reqFindUser(text);
}

// 问题：没有根据不同错误类型弹出不同对话框
// 追加需求：用户名补在窗口里

// 查找用户结果
void FriendWindow::onFindUserResult(const char* caName, int res)
{
    if (res ==  FIND_USER_FAIL_NOT_FOUND)
    {
        QMessageBox::information(this, "提示", QString("用户 %1 未找到").arg(caName));
    }
    else if(res ==  FIND_USER_SUCCESS_OFFLINE)
    {
        QMessageBox::information(this, "提示", QString("用户 %1 不在线").arg(caName));
    }
    else if(res ==  FIND_USER_SUCCESS_ONLINE)
    {
        int ret=QMessageBox::information(this, "提示", QString("用户 %1 在线").arg(caName),"添加好友","取消");
        qDebug()<<"ret:"<<ret;
        if(ret==0){
            Client::getInstance().reqAddFriend(caName);
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", QString("查询错误").arg(caName));
    }
}

void FriendWindow::onAddFriendResult(int res)
{
   if(res == ADD_FRIEND_FAIL_SQL_ERR || res == ADD_FRIEND_FAIL_NAME_ERR){
       QMessageBox::warning(this,"提示",QString("服务器内部问题,请联系开发人员"));
   }
   else if(res == ADD_FRIEND_FAIL_FRIEND){
       QMessageBox::information(this,"提示",QString("对方已经是你的好友!"));
   }
   else if(res == ADD_FRIEND_FAIL_OFFLINE){
       QMessageBox::information(this,"提示",QString("对方不在线,稍后添加"));
   }
}
void FriendWindow::onAddFriendResendResult(const char *caCurName,const char *caTarName)
{
    int ret = QMessageBox::question(this,"提示",QString("是否同意 %1 的好友申请 ?").arg(caCurName));
    if (ret!=QMessageBox::Yes){
        return;
    }
    Client::getInstance().reqAddFriendAgree(caCurName,caTarName);
}

void FriendWindow::onAddFriendAgreeResult(bool res)
{
    if(res){
        QMessageBox::information(this, "提示","添加好友成功");
    }else{
        QMessageBox::information(this, "提示","添加好友失败");
    }
    Client::getInstance().reqFlushFriend();
}
void FriendWindow::on_btnRefresh_clicked()
{
    Client::getInstance().reqFlushFriend();
}
void FriendWindow::updateListWidget (QStringList strNameList)
{
    ui->listWidget->clear ();
    ui->listWidget->addItems(strNameList);
}

void FriendWindow::onDeleteFriendResult(bool res)
{
    if(res){
        Client::getInstance().reqFlushFriend();
    }else{
        QMessageBox::information(this, "提示","删除好友失败");
    } 
}
void FriendWindow::on_btnDelete_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(!pItem){
        return;
    }
    Client::getInstance().reqDeleteFriend(pItem->text());
}

void FriendWindow::on_btnchat_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(!pItem){
        return;
    }
    if(m_pChat->isHidden()){
        m_pChat->show();
    }
    m_pChat->m_strChatName=pItem->text();
    qDebug()<<"点击聊天按钮,要聊天的好友为:"<<m_pChat->m_strChatName;
    m_pChat->setWindowTitle(QString("聊天界面:%1").arg(m_pChat->m_strChatName));
}
