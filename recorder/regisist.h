#ifndef REGISIST_H
#define REGISIST_H

#include <QMainWindow>
#include "farm_login.h"
namespace Ui {
class regisist;
}

class regisist : public QMainWindow
{
    Q_OBJECT

public:
    explicit regisist(QWidget *parent = nullptr);
    ~regisist();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::regisist *ui;
};

#endif // REGISIST_H
