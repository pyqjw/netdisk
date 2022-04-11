#ifndef FARM_LOGIN_H
#define FARM_LOGIN_H
#include "regisist.h"
#include <QMainWindow>
#include <mainwindow.h>
QT_BEGIN_NAMESPACE
namespace Ui { class farm_login; }
QT_END_NAMESPACE

class farm_login : public QMainWindow
{
    Q_OBJECT

public:
    farm_login(QWidget *parent = nullptr);
    ~farm_login();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::farm_login *ui;
};
#endif // FARM_LOGIN_H
