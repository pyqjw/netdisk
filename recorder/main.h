#ifndef MAIN_H
#define MAIN_H
#include <QApplication>
#include <QMessageBox>
#include <string>
#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QStackedWidget>
#include <unistd.h>
#include "mainwindow.h"
using namespace std;

#define LOGIN_FUC 1
#define REGIST_FUC 2
#define ISEARCH 3
#define IUPLOAD 4
#define IDELETE 5
#define IDOWNLOAD 6
#define INEWFOLDER 7
#define IRENAME 8
extern char userName[20];
extern char serverIp[20];
extern const int DEFAULT_PORT;
extern SOCKADDR_IN addrSrv;
extern MainWindow *mw;
#endif // MAIN_H
