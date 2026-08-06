#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "client.h"
#include "friendwindow.h"
#include "index.h"
#include "reshandler.h"

Loginwindow::Loginwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Loginwindow)
{
    ui->setupUi(this);
    connect(
                Client::getInstance().getResHandler(),
                &ResHandler::signalLoginResult,
                this,
                &Loginwindow::onLoginResult
                );
    connect(
                Client::getInstance().getResHandler(),
                &ResHandler::signalRegisterResult,
                this,
                &Loginwindow::onregisterResult
                );
}

Loginwindow::~Loginwindow()
{
    delete ui;
}

//注册
void Loginwindow::on_pushButton_clicked()
{
    //将账号密码发送给服务端
    QString strName = ui->lineEditName->text();
    QString strPwd = ui->lineEditPwd->text();

    //判断是否合法
    if(strName.toStdString().length()>=32
            ||strPwd.toStdString().length()>=32
            ||strName.toStdString().length()==0
            ||strPwd.toStdString().length()==0
            )
    {
        //提示用户名密码过长
       QMessageBox::warning(this,"警告","用户名或密码非法");
        return;
    }
    Client::getInstance().m_strLoginName = strName;
    Client::getInstance().reqLogin(strName, strPwd);
    //qDebug()<<strUsername<<strPwd;
}
//登录
void Loginwindow::on_pushButton_2_clicked()
{
    QString strName = ui->lineEditName->text();
    QString strPwd = ui->lineEditPwd->text();

    //判断是否合法
    if(strName.toStdString().length()>=32
            ||strPwd.toStdString().length()>=32
            ||strName.toStdString().length()==0
            ||strPwd.toStdString().length()==0
            )
    {
        //提示用户名密码过长
       QMessageBox::warning(this,"警告","用户名或密码非法");
        return;
    }
     Client::getInstance().reqRegister(strName,strPwd);
    //qDebug()<<strUsername<<strPwd;
}
void Loginwindow::onLoginResult(bool res)
{
    if(res)
    {
    QMessageBox::information(this,"提示","登陆成功");
    Index *index = new Index();
    index->show();
    hide();
    }
    else
    {
     QMessageBox::warning(this,"警告","登陆失败!!!");
    }
}
void Loginwindow::onregisterResult(bool res)
{
    if(res)
    {
    QMessageBox::information(this,"完成","注册成功");
    }
    else
    {
     QMessageBox::warning(this,"警告","当前用户已经注册");
    }
}
