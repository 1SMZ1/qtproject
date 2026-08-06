#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Loginwindow; }
QT_END_NAMESPACE

class Loginwindow : public QWidget
{
    Q_OBJECT

public:
    Loginwindow(QWidget *parent = nullptr);
    ~Loginwindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void onLoginResult(bool res);
    void onregisterResult(bool res);
private:
    Ui::Loginwindow *ui;
};
#endif // LOGINWINDOW_H
