#include "chat.h"
#include "client.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    connect(Client::getInstance().getResHandler(), &ResHandler::signalChatResult, this, &Chat::updateshow_TE);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_send_PB_clicked()
{
    QString strmsg = ui->input_LE->text();
    if(strmsg.isEmpty()){
        return;
    }
    ui->input_LE->clear();
    Client::getInstance().reqChat(m_strChatName,strmsg);

}

void Chat::updateshow_TE(QString strName,QString strmsg)
{
    if(this->isHidden()){
        this->show();
    }
    m_strChatName = strName;
    ui->show_TE->append(QString("%1:%2").arg(strName).arg(strmsg));

}
