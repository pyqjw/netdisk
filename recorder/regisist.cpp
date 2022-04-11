#include "regisist.h"
#include "ui_regisist.h"
#include <QDebug>
#include <QCompleter>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QStringLiteral>
#include "main.h"
regisist::regisist(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::regisist)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
}

regisist::~regisist()
{
    delete ui;
}

void regisist::on_pushButton_clicked()
{
    QString name=ui->lineEdit->text();
    QString pwd=ui->lineEdit_2->text();
    QString sex=ui->radioButton->isChecked() ? "Man" : "Woman";
    QString birthday=ui->dateEdit->text();
    QString tel=ui->lineEdit_3->text();


    if((!name.isEmpty())&&(!pwd.isEmpty()))
    {
       //把name、pwd传过去
        SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
        ::connect(client,(SOCKADDR*)&addrSrv, sizeof(addrSrv));
        struct RegistStr {
            char username[32];
            char password[32];
        }registstr;
        sprintf(registstr.username, "%s", name.toStdString().c_str());
        sprintf(registstr.password, "%s", pwd.toStdString().c_str());
        int order = REGIST_FUC;
        send(client, (char*)&order, sizeof(int), 0);
        send(client,(char*)&registstr,sizeof(RegistStr), 0);
        int flag;
        recv(client, (char*)&flag, sizeof(int), 0);
        if(flag) {
            QMessageBox::information(this, "提示", "注册成功", QMessageBox::Ok);
        } else {
            QMessageBox::information(this, "提示", "注册失败", QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, "提示", "注册失败", QMessageBox::Ok);
    }
}


void regisist::on_pushButton_2_clicked()
{
    farm_login *reg = new farm_login();
    reg->show();
    this->close();
}

