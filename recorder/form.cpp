#include "form.h"
#include "ui_form.h"
#include "main.h"
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}
void Form::Init(QString fname, int isDir, int index) {
    this->fname = fname;
    this->isDir = isDir;
    this->index = index;
    ui->namelabel->setText(fname);
    if(isDir == 1) {
        ui->typelabel->setText("文件夹");
        ui->download->setDisabled(true);
    } else {
        ui->typelabel->setText("文件");
        ui->openButton->setDisabled(true);
    }
}
Form::~Form()
{

    delete ui;
}

void Form::on_openButton_clicked()
{
    mw->openFolder(index);
}


void Form::on_deleteButton_clicked()
{
    mw->deleteFileOrFolder(index);
}


void Form::on_download_clicked()
{
    mw->downloadFile(index);
}


void Form::on_rename_clicked()
{
    mw->renameFile(index);
}

