#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPalette>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include "form.h"
#include <bits/stdc++.h>
#define MAX_PATHLEN 4096
using namespace std;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int xxxx;
    bool isrecording=0;
    time_t rawtime;
    char outputname[MAX_PATHLEN*4];
    time_t clockstartt;
    QString path;
    QString recPath;
    MainWindow(QWidget *parent = nullptr);
    QVBoxLayout *layout;
    vector<Form*>  vfiles;
    vector<QString*> vpaths;
    void getFiles();
    ~MainWindow();
    int start_rec();
    void openFolder(int index);
    void deleteFileOrFolder(int index);
    void downloadFile(int index);
    void renameFile(int index);
private slots:
    void on_action_about_triggered();

    void on_action_recorder_triggered();

    void on_action_disk_triggered();

    void on_recordButton_clicked();

    void on_MainWindow_destroyed();

    void on_savePath_clicked();

    void on_pushButton_clicked();

    void on_uploadButton_clicked();

    void on_newDir_clicked();

    void on_fresh_clicked();

    void on_upperFolder_clicked();

private:
    void init();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
