#ifndef FORM_H
#define FORM_H

#include <QWidget>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    QString fname;
    int isDir;
    int index;
    explicit Form(QWidget *parent = nullptr);
    void Init(QString fname, int isFile, int index);
    ~Form();
private slots:
    void on_openButton_clicked();
    void on_deleteButton_clicked();
    void on_download_clicked();

    void on_rename_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
