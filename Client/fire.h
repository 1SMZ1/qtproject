#ifndef FIRE_H
#define FIRE_H

#include <QWidget>

namespace Ui {
class Fire;
}

class Fire : public QWidget
{
    Q_OBJECT

public:
    explicit Fire(QWidget *parent = nullptr);
    ~Fire();
    QString m_strUserPath;
    QString m_strCurPath;
    QString m_strUploadPath;
    void on_mkdir_PB_clicked();
    void onMKDirResult(bool res);
    void on_upload_PB_clicked();
    void onUploadDataResult(bool res);
private:
    Ui::Fire *ui;
};

#endif // FIRE_H
