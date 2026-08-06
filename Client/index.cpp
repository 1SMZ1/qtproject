#include "index.h"
#include "ui_index.h"
#include "loginwindow.h"
#include <QMessageBox>
Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);
}

Index::~Index()
{
    delete ui;
}

void Index::on_friend_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->friend_PB->setStyleSheet("QPushButton{background-color:rgb(255, 255, 255) ; border:none;padding:5px;}");
    ui->fire_PB->setStyleSheet("QPushButton{background-color:rgba(255, 255, 255,0) ; border:none;padding:5px;}");
}

void Index::on_fire_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->fire_PB->setStyleSheet("QPushButton{background-color:rgb(255, 255, 255) ; border:none;padding:5px;}");
    ui->friend_PB->setStyleSheet("QPushButton{background-color:rgba(255, 255, 255,0) ; border:none;padding:5px;}");
}

