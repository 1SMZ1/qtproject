#include "fire.h"
#include "ui_fire.h"
#include "client.h"

#include <qinputdialog.h>
#include <qmessagebox.h>
Fire::Fire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Fire)
{
    ui->setupUi(this);
    m_strUserPath = QString("%1/%2").arg(Client::getInstance().m_strRootPath).arg(Client::getInstance().m_strLoginName);
    m_strCurPath = m_strUserPath;
        qDebug()<<"m_strUserPath"<<m_strUserPath;
}

Fire::~Fire()
{
    delete ui;
}

void Fire::on_mkdir_PB_clicked(){
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","新建文件夹名");
    if(strNewDir.isEmpty()||strNewDir.length()>32){
        QMessageBox::information(this,"新建文件夹","新建文件夹名长度非法");
        return;
    }
    Client::getInstance().reqMKDir(m_strCurPath,strNewDir);
}

void Fire::onMKDirResult(bool res){
    if(res){
        QMessageBox::information(this,"提示","创建文件夹成功");
    }else{
        QMessageBox::information(this,"提示","创建文件夹失败");
    }
}
void Fire::on_listWidget_itemDoubleClicked()_
void Fire::on_upload_PB_clicked(){
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","新建文件夹名");
    if(strNewDir.isEmpty()||strNewDir.length() > 32){
        QMessageBox::information(this,"新建文件夹","新建文件夹名长度非法");
        return;
    }
    Client::getInstance().reqMKDir(m_strCurPath,strNewDir);
}
void Fire::onUploadDataResult(bool res){
    if(res){
        Client::getInstance().reqFlushFile(m_strCurPath);
    }
}
