#ifndef FRIENDWINDOW_H
#define FRIENDWINDOW_H

#include <QWidget>
#include <chat.h>
namespace Ui {
class FriendWindow;
}

class FriendWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FriendWindow(QWidget *parent = nullptr);
    ~FriendWindow();
    Chat* m_pChat;



private slots:
    void on_btnFindUser_clicked();
    void on_btnRefresh_clicked();
    void onFindUserResult(const char* caName, int res);
    void onAddFriendResult(int res);
    void onAddFriendResendResult(const char *caCurName, const char *caTarName);
    void onAddFriendAgreeResult(bool res);
    void updateListWidget(QStringList strNameList);
    void onDeleteFriendResult(bool res);

    //void on_btnAddfriend_clicked();

    void on_btnDelete_clicked();

    void on_btnchat_clicked();

private:
    Ui::FriendWindow *ui;
};

#endif // FRIENDWINDOW_H
