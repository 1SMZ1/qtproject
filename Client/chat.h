#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    QString m_strChatName;



private slots:
    void on_send_PB_clicked();
    void updateshow_TE(QString strName, QString strmsg);


private:
    Ui::Chat *ui;
};

#endif // CHAT_H
